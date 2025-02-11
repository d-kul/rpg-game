#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

class Entity : public sf::Transformable, public sf::Drawable {
 public:
  Entity();
  virtual ~Entity();

  virtual void update(sf::Time dt) = 0;

  virtual void move(sf::Vector2f offset);
  virtual void rotate(sf::Angle angle);
  virtual void scale(sf::Vector2f factor);
};
