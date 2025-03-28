#include "TileMapCollider.h"

#include <cmath>

TileMapCollider::TileMapCollider(ColliderManager& manager)
    : Collider(manager) {}

void TileMapCollider::load(std::vector<bool> tileColliderMap, float tileSize,
                           sf::Vector2u size) {
  this->tileColliderMap = std::move(tileColliderMap);
  this->size = size;
  this->tileSize = tileSize;
}

bool TileMapCollider::checkCollision(sf::Vector2f point) {
  auto coords = sf::Vector2i{static_cast<int>(std::floor(point.x / tileSize)),
                             static_cast<int>(std::floor(point.y / tileSize))};
  if (tileColliderMap.empty() || coords.x < 0 || size.x <= coords.x ||
      coords.y < 0 || size.y <= coords.y) {
    return false;
  }
  return tileColliderMap[coords.x + coords.y * size.x];
}
