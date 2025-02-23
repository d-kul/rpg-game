#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

class Background : public sf::Transformable, public sf::Drawable {
 public:
  Background();

  void setTexture(sf::Texture* texture, bool repeated = false);
  void setTexture(sf::Texture* texture, sf::Vector2f textureSize,
                  bool repeated = false);
  void unsetTexture();
  void setTextureSize(sf::Vector2f size);
  void setMoving(bool moving);
  void setView(sf::View view);

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::RectangleShape shape;
  sf::Vector2f textureScaling = {1.f, 1.f};
  bool moving = false;
};
