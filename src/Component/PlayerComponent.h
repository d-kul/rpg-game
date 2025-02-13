#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Clock.hpp>

class PlayerComponent {
 public:
  sf::SoundBuffer& soundBuffer;
  sf::Clock clock{};
  float movementSpeed = 600.f;
};
