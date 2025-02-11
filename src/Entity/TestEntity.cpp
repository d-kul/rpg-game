#include "TestEntity.h"

TestEntity::TestEntity(const sf::Texture& texture) : sprite(texture) {
  sprite.setScale({0.7f, 0.7f});
}

TestEntity::~TestEntity() {}

void TestEntity::update(sf::Time dt) {}

void TestEntity::move(sf::Vector2f offset) {
  Entity::move(offset * movementSpeed);
}

void TestEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite, states);
}
