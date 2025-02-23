#pragma once

#include <SFML/Graphics/Text.hpp>

#include "Element.h"

class Text : public UIElement {
 public:
  Text(const sf::Font& font, sf::String string = "",
       unsigned int characterSize = 30);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  sf::Text text;
};
