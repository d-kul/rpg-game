#pragma once

#include "Entity/Collider.h"

class TileMapCollider : public Collider {
 public:
  void load(const bool* tileColliderMap, float tileSize, sf::Vector2u size);
  bool checkCollision(sf::Vector2f point) override;

 private:
  const bool* tileColliderMap = nullptr;
  sf::Vector2u size;
  float tileSize;
};
