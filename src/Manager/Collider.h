#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

class ColliderManager;

#include "Entity/Collider.h"

class ColliderManager {
 public:
  void addCollider(Collider& collider);
  void removeCollider(Collider& collider);
  bool checkCollision(sf::Vector2f point);

 private:
  std::vector<Collider*> colliders;
};
