#pragma once

#include <SFML/System/Vector2.hpp>

class Collider;

#include "Manager/Collider.h"

class Collider {
 public:
  Collider(ColliderManager& manager);
  virtual ~Collider();

  virtual bool checkCollision(sf::Vector2f point) = 0;

 private:
  ColliderManager& manager;
};
