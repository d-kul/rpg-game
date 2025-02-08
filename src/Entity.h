#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

class Entity {
 private:
 protected:
  virtual const sf::Transformable& asTransformable() const = 0;
  virtual sf::Transformable& asTransformable() = 0;

 public:
  Entity();
  virtual ~Entity();

  // Functionality
  virtual void move(sf::Vector2f offset);
  virtual void rotate(sf::Angle angle);
  virtual void scale(sf::Vector2f factor);

  virtual void update(sf::Time dt) = 0;
  virtual void render(sf::RenderTarget& target) = 0;
};
