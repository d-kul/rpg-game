#include "Player.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Game.h"

Player::Player(float movementSpeed)
    : keybinds(Game::getKeybinds()),
      audioManager(Game::getAudioManager()),
      soundBuffer(Game::getResourceManager().load<sf::SoundBuffer>(
          "sounds/vine_boom", "resources/sounds/vine_boom.wav")),
      texture(Game::getResourceManager().load<sf::Texture>(
          "textures/pearto", "resources/images/pearto.png")),
      sprite(*texture),
      movementSpeed(movementSpeed) {
  sprite.setOrigin(sprite.getGlobalBounds().getCenter());
}

void Player::update(sf::Time dt) {
  if (soundClock.getElapsedTime() > EMIT_FREQ &&
      sf::Keyboard::isKeyPressed(keybinds["MAKE_SOUND"])) {
    soundClock.restart();
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
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite, states);
}
