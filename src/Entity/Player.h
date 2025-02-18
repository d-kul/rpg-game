#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include "AudioManager.h"
#include "Core/utility.h"
#include "Entity.h"

class Player : public Entity {
 public:
  static constexpr sf::Time EMIT_FREQ = sf::seconds(0.07);

 public:
  Player(float movementSpeed = 500.f);

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  keybinds_t& keybinds;
  AudioManager& audioManager;
  sf::Clock soundClock;

  std::shared_ptr<sf::SoundBuffer> soundBuffer;
  std::shared_ptr<sf::Texture> texture;
  sf::Sprite sprite;
  float movementSpeed;
};
