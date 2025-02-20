#include "RectCollider.h"

RectCollider::RectCollider(sf::FloatRect rect) : rect(rect) {}

void RectCollider::setRect(sf::FloatRect rect) { this->rect = rect; }

bool RectCollider::checkCollision(sf::Vector2f point) {
  return rect.contains(point);
}
