#include "Player.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

#include "Game.h"

Player::Player(float movementSpeed, float tileSize)
    : keybinds(Game::getKeybinds()),
      audioManager(Game::getAudioManager()),
      soundBuffer(Game::getResourceManager().retain<sf::SoundBuffer>(
          "sounds/vine_boom", "resources/sounds/vine_boom.wav")),
      spriteSheet(Game::getResourceManager().retain<TileSet>(
          "textures/omori_sheet", sf::Vector2i{32, 32},
          "resources/images/omori_sheet.png")),
      sprite(*spriteSheet),
      movementSpeed(movementSpeed),
      tileSize(tileSize) {
  sprite.setOrigin(sprite.getGlobalBounds().getCenter());
  sprite.setScale({tileSize / 32.f, tileSize / 32.f});
  movementDestination = getPosition();
  updateAnimation();
}

void Player::update(sf::Time dt) {
  if ((elapsedTime += dt) > EMIT_FREQ &&
      sf::Keyboard::isKeyPressed(keybinds["MAKE_SOUND"])) {
    elapsedTime = sf::Time::Zero;
    audioManager.playSound(*soundBuffer);
  }

  sf::Vector2f movement;
  bool isRunning = sf::Keyboard::isKeyPressed(keybinds["RUN"]);
  float speed = isRunning ? movementSpeed * 1.5f : movementSpeed;
  if ((getPosition() - movementDestination).lengthSquared() < 0.05f) {
    if (sf::Keyboard::isKeyPressed(keybinds["MOVE_UP"])) {
      movement.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(keybinds["MOVE_DOWN"])) {
      movement.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(keybinds["MOVE_LEFT"])) {
      movement.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(keybinds["MOVE_RIGHT"])) {
      movement.x += 1.f;
    }
    movementDestination = getPosition();
    // TODO: magic numbers
    if (movement.x != 0) {
      movementDestination.x += movement.x * tileSize;
      setAnimationDirection(movement.x > 0 ? Right : Left, isRunning);
    } else if (movement.y != 0) {
      movementDestination.y += movement.y * tileSize;
      setAnimationDirection(movement.y > 0 ? Down : Up, isRunning);
    } else {
      unsetAnimationIdle();
    }
  } else {
    movement = movementDestination - getPosition();
    if (movement.length() + 0.05f > speed * dt.asSeconds()) {
      movement = movement.normalized();
      movement *= speed * dt.asSeconds();
    }
    move(movement);
  }

  sprite.update(dt);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite, states);
}

void Player::setAnimationDirection(animationDirection_t direction,
                                   bool isRunning) {
  auto prevDirection = animationDirection;
  bool prevIdle = animationIdle;
  bool prevRunning = animationRunning;
  animationDirection = direction;
  animationIdle = false;
  animationRunning = isRunning;
  if (prevDirection != direction || prevIdle || prevRunning != isRunning) {
    updateAnimation();
  }
}

void Player::unsetAnimationIdle() {
  bool prevIdle = animationIdle;
  animationIdle = true;
  if (!prevIdle) {
    updateAnimation();
  }
}

static const int animations[] = {
    0,  1,  2,  1,                   // Down
    8,  9,  10, 9,                   // Left
    16, 17, 18, 17,                  // Right
    24, 25, 26, 25,                  // Up
    32, 33, 34, 35, 36, 37, 38, 39,  // Running Down
    40, 41, 42, 43, 44, 45, 46, 47,  // Running Left
    48, 49, 50, 51, 52, 53, 54, 55,  // Running Right
    56, 57, 58, 59, 60, 61, 62, 63,  // Running Up
};

void Player::updateAnimation() {
  const int* frames = animationRunning && !animationIdle
                          ? &animations[16 + animationDirection * 8]
                          : &animations[animationDirection * 4];
  if (animationIdle) {
    sprite.setFrames(frames + 1, 1);
  } else if (animationRunning) {
    sprite.setFrames(frames, 8);
    sprite.setAnimationFrequency(sf::seconds(0.0833f));
  } else {
    sprite.setFrames(frames, 4);
    sprite.setAnimationFrequency(sf::seconds(0.25f));
  }
}
