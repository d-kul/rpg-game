#pragma once

#include <SFML/System/Vector2.hpp>

class Collider;

#include "Manager/Collider.h"

class Collider {
 public:
  Collider();
  virtual ~Collider();

  virtual bool checkCollision(sf::Vector2f point) = 0;

 private:
  ColliderManager& manager;
};
