#include "Entity.h"

Entity::Entity() {}

Entity::~Entity() {}

void Entity::move(sf::Vector2f offset) { asTransformable().move(offset); }

void Entity::rotate(sf::Angle angle) { asTransformable().rotate(angle); }

void Entity::scale(sf::Vector2f factor) { asTransformable().scale(factor); }
