#include "RectCollider.h"

RectCollider::RectCollider(ColliderManager& manager, sf::FloatRect rect)
    : Collider(manager), rect(rect) {}

void RectCollider::setRect(sf::FloatRect rect) { this->rect = rect; }

bool RectCollider::checkCollision(sf::Vector2f point) {
  return rect.contains(point);
}
