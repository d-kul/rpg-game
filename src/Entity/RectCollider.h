#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "Entity/Collider.h"

class RectCollider : public Collider {
 public:
  RectCollider(ColliderManager& manager, sf::FloatRect rect = {});

  void setRect(sf::FloatRect rect);
  bool checkCollision(sf::Vector2f point) override;

 private:
  sf::FloatRect rect;
};
