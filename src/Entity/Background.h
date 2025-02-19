#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include "Entity.h"

class Background : public Entity {
 public:
  Background(bool moving = false);

  void setTexture(sf::Texture* texture, bool repeated = false);
  void setTexture(sf::Texture* texture, sf::Vector2f textureSize,
                  bool repeated = false);
  void setSize(sf::Vector2f size);
  void setMoving(bool moving);

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::RenderWindow& window;
  sf::RectangleShape shape;
  sf::Vector2f textureScaling;
  bool moving;
};
