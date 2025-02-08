#include "TestEntity.h"

#include <SFML/Window/Keyboard.hpp>

const sf::Transformable& TestEntity::asTransformable() const { return shape; }

sf::Transformable& TestEntity::asTransformable() { return shape; }

TestEntity::TestEntity() { shape.setTexture(&texture); }

TestEntity::~TestEntity() {}

void TestEntity::move(sf::Vector2f offset) {
  asTransformable().move(offset * movementSpeed);
}

void TestEntity::update(sf::Time dt) {}

void TestEntity::render(sf::RenderTarget& target) { target.draw(shape); }
