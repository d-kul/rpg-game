#include "Player.h"

#include <SFML/Window/Keyboard.hpp>
#include <cmath>

#include "Game.h"

Player::Player(float tileSize, float movementSpeed)
    : keybinds(Game::getKeybinds()),
      window(Game::getWindow()),
      audioManager(Game::getAudioManager()),
      interactibleManager(Game::getInteractibleManager()),
      colliderManager(Game::getColliderManager()),
      spriteSheet(Game::getResourceManager().retain<TileSet>(
          "textures/omori_sheet", 32, "resources/images/omori_sheet.png")),
      sprite(*spriteSheet),
      tileSize(tileSize),
      movementSpeed(movementSpeed) {
  sprite.setScale({tileSize / 32.f, tileSize / 32.f});
  updateAnimation();
}

void Player::setPosition(sf::Vector2f position) {
  sf::Transformable::setPosition(position);
  setDestination(position);
}

void Player::setDestination(sf::Vector2f destination) {
  movementDestination = destination;
}

void Player::update(sf::Time dt) {
  updateInput();
  updateMovement(dt);
  sprite.update(dt);
  auto view = window.getView();
  view.setCenter(getPosition() + sprite.getGlobalBounds().getCenter());
  window.setView(view);
  if (sf::Keyboard::isKeyPressed(keybinds["INTERACT"])) {
    if (!pressed) {
      switch (animationState.direction) {
        case AnimationState::Up:
          interactibleManager.interact(sf::Vector2i{getPosition() / tileSize} +
                                       sf::Vector2i{0, -1});
          break;
        case AnimationState::Down:
          interactibleManager.interact(sf::Vector2i{getPosition() / tileSize} +
                                       sf::Vector2i{0, 1});
          break;
        case AnimationState::Left:
          interactibleManager.interact(sf::Vector2i{getPosition() / tileSize} +
                                       sf::Vector2i{-1, 0});
          break;
        case AnimationState::Right:
          interactibleManager.interact(sf::Vector2i{getPosition() / tileSize} +
                                       sf::Vector2i{1, 0});
          break;
      }
    }
    pressed = true;
  } else {
    pressed = false;
  }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite, states);
}

void Player::updateInput() {
  sf::Vector2f newInput;
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_UP"])) {
    newInput.y -= 1.f;
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_DOWN"])) {
    newInput.y += 1.f;
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_LEFT"])) {
    newInput.x -= 1.f;
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_RIGHT"])) {
    newInput.x += 1.f;
  }
  if (newInput.y != 0 && newInput.x != 0) {
    if (input.y != 0) {
      newInput.x = 0;
    } else {
      newInput.y = 0;
    }
  }
  input = newInput;
}

void Player::updateMovement(sf::Time dt) {
  bool running = sf::Keyboard::isKeyPressed(keybinds["RUN"]);
  float speed = running ? movementSpeed * 1.5f : movementSpeed;
  auto path_left = movementDestination - getPosition();
  float step = speed * dt.asSeconds();
  if (path_left.length() < CONTROL_RANGE ||
      step > path_left.length() + CONTROL_RANGE) {
    setPosition(movementDestination);
    if (input.length() == 0 ||
        !colliderManager.checkCollision(getPosition() + input * tileSize +
                                        sf::Vector2f{0.5f, 0.5f} * tileSize)) {
      move(input * (step - path_left.length()));
      movementDestination += input * tileSize;
    }
    updateAnimationState();
  } else {
    move(path_left.normalized() * step);
  }
}

void Player::updateAnimationState() {
  auto prevAnimationState = animationState;
  bool running = sf::Keyboard::isKeyPressed(keybinds["RUN"]);
  if (input.x != 0) {
    animationState.direction =
        input.x > 0 ? AnimationState::Right : AnimationState::Left;
  } else if (input.y != 0) {
    animationState.direction =
        input.y > 0 ? AnimationState::Down : AnimationState::Up;
  }
  if ((movementDestination - getPosition()).length() < CONTROL_RANGE) {
    animationState.idle = true;
  } else {
    animationState.idle = false;
    animationState.running = running;
  }
  if (prevAnimationState.direction != animationState.direction ||
      prevAnimationState.idle != animationState.idle ||
      prevAnimationState.running != animationState.running) {
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
  const int* frames = animationState.running && !animationState.idle
                          ? &animations[16 + animationState.direction * 8]
                          : &animations[animationState.direction * 4];
  if (animationState.idle) {
    sprite.setFrames(frames + 1, 1);
  } else if (animationState.running) {
    sprite.setFrames(frames, 8);
    sprite.setAnimationFrequency(sf::seconds(0.0833f));
  } else {
    sprite.setFrames(frames, 4);
    sprite.setAnimationFrequency(sf::seconds(0.25f));
  }
}
