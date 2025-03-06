#include "Text.h"

Text::Text(const sf::Font& font, sf::String string, unsigned int characterSize)
    : text(font, string, characterSize) {}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (!active) return;
  states.transform *= getTransform();
  target.draw(text, states);
}
