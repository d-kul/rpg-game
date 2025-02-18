#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <memory>

class Entity : public sf::Transformable, public sf::Drawable {
 public:
  virtual ~Entity() {}
  virtual void update(sf::Time dt) {}
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {}
};

using entity_t = std::unique_ptr<Entity>;
