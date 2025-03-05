#include "Collider.h"

#include <algorithm>

#include "Core/Logger.h"

void ColliderManager::addCollider(Collider& collider) {
  DEBUG("added collider ", &collider);
  colliders.push_back(&collider);
}

void ColliderManager::removeCollider(Collider& collider) {
  DEBUG("removed collider ", &collider);
  colliders.erase(std::find(colliders.begin(), colliders.end(), &collider));
}

bool ColliderManager::checkCollision(sf::Vector2f point) {
  return std::any_of(
      colliders.begin(), colliders.end(),
      [&](Collider* collider) { return collider->checkCollision(point); });
}
