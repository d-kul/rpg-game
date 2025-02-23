#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Grid : public sf::Drawable {
 public:
  Grid(sf::Vector2f spacing, sf::Vector2f offset = {},
       sf::Color color = sf::Color::Green);

  void setSpacing(sf::Vector2f spacing);
  void setOffset(sf::Vector2f offset);
  void setColor(sf::Color color);

  void setSize(sf::Vector2f size);
  void setPosition(sf::Vector2f position);
  void setView(const sf::View& view);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::Vector2f spacing, size, offset, centerPosition;
  sf::Color color;
  sf::VertexArray vertices;
};
