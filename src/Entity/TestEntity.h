#pragma once

#include <SFML/Graphics.hpp>

#include "Entity.h"

class TestEntity : public Entity {
 private:
  const sf::Transformable& asTransformable() const;
  sf::Transformable& asTransformable();

 public:
  TestEntity();
  ~TestEntity();

  void move(sf::Vector2f offset);

  void update(sf::Time dt);
  void render(sf::RenderTarget& target);

 private:
  sf::Texture texture{"resources/pearto.png"};
  sf::RectangleShape shape{{100.f, 100.f}};
  float movementSpeed = 300.f;
};
