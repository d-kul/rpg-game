#pragma once

#include <SFML/System/Vector2.hpp>

class MovementComponent {
 public:
  sf::Vector2f velocity;
  sf::Vector2f acceleration;
};
