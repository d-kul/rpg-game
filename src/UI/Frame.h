#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Element.h"

class Frame : public UIElement {
 public:
  Frame();
  bool handleEvent(sf::Event event) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  sf::RectangleShape shape;
};
