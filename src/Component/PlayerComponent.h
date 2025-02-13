#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Clock.hpp>
#include <entt/entt.hpp>

class PlayerComponent {
 public:
  entt::resource<sf::SoundBuffer> soundBuffer;
  sf::Clock clock{};
  float movementSpeed = 600.f;
};
