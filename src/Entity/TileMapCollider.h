#pragma once

#include "Entity/Collider.h"

class TileMapCollider : public Collider {
 public:
  void load(std::vector<bool> tileColliderMap, float tileSize, sf::Vector2u size);
  bool checkCollision(sf::Vector2f point) override;

 private:
  std::vector<bool> tileColliderMap;
  sf::Vector2u size;
  float tileSize;
};
