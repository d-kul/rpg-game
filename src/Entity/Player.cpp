#include "Player.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Game.h"

Player::Player(float movementSpeed)
    : keybinds(Game::getKeybinds()),
      audioManager(Game::getAudioManager()),
      soundBuffer(Game::getResourceManager().retain<sf::SoundBuffer>(
          "sounds/vine_boom", "resources/sounds/vine_boom.wav")),
      spriteSheet(Game::getResourceManager().retain<TileSet>(
          "textures/comic mono", sf::Vector2i{32, 32},
          "resources/images/comic mono.png")),
      sprite(*spriteSheet),
      movementSpeed(movementSpeed) {
  sprite.setFrames({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
  sprite.setOrigin(sprite.getGlobalBounds().getCenter());
  sprite.setScale({3.f, 3.f});
}

void Player::update(sf::Time dt) {
  if ((elapsedTime += dt) > EMIT_FREQ &&
      sf::Keyboard::isKeyPressed(keybinds["MAKE_SOUND"])) {
    elapsedTime = sf::Time::Zero;
    audioManager.playSound(*soundBuffer);
  }

  sf::Vector2f movement;
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
  if (movement.lengthSquared() > 0.f) {
    movement = movement.normalized();
  }
  movement *= movementSpeed * dt.asSeconds();
  move(movement);

  sprite.update(dt);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite, states);
}
