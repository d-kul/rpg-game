#include "Entity.h"

Entity::Entity() {}

Entity::~Entity() {}

void Entity::move(sf::Vector2f offset) {
  sf::Transformable::move(offset);
}

void Entity::rotate(sf::Angle angle) {
  sf::Transformable::rotate(angle);
}

void Entity::scale(sf::Vector2f factor) {
  sf::Transformable::scale(factor);
}
