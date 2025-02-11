#pragma once

#include <SFML/Graphics.hpp>

#include "Entity.h"

class TestEntity : public Entity {
 private:
  sf::Sprite sprite;
  float movementSpeed = 600.f;

 public:
  TestEntity(const sf::Texture& texture);
  ~TestEntity();

  void update(sf::Time dt) override;

  void move(sf::Vector2f offset) override;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
