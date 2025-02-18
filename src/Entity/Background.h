#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <memory>

#include "Entity.h"

class Background : public Entity {
 public:
  Background();
  Background& setTexture(std::shared_ptr<sf::Texture> texture);
  Background& setTextureRect(sf::IntRect rect);
  Background& setSize(sf::Vector2f size);
  Background& setMoving(bool moving);

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::RenderWindow& window;
  std::shared_ptr<sf::Texture> texture;
  sf::RectangleShape shape;
  bool moving = true;
};
