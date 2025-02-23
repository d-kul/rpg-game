#include "Background.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

Background::Background() {}

void Background::setTexture(sf::Texture* texture, bool repeated) {
  setTexture(texture, sf::Vector2f{texture->getSize()}, repeated);
}

void Background::setTexture(sf::Texture* texture, sf::Vector2f textureSize,
                            bool repeated) {
  shape.setTexture(texture);
  setTextureSize(textureSize);
  texture->setRepeated(repeated);
}

void Background::setTextureSize(sf::Vector2f size) {
  textureScaling =
      size.componentWiseDiv(sf::Vector2f{shape.getTexture()->getSize()});
}

void Background::setMoving(bool moving) {
  this->moving = moving;
  if (!moving) {
    shape.setTextureRect(sf::IntRect{sf::FloatRect{
        {0, 0}, shape.getSize().componentWiseDiv(textureScaling)}});
  }
}

void Background::setView(sf::View view) {
  shape.setSize(view.getSize());
  shape.setOrigin(shape.getGeometricCenter());
  if (moving) {
    auto top_left = view.getCenter() - view.getSize() / 2.f;
    top_left = top_left.componentWiseDiv(textureScaling);
    shape.setTextureRect(sf::IntRect{sf::FloatRect{
        top_left, shape.getSize().componentWiseDiv(textureScaling)}});
  }
  setPosition(view.getCenter());
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
}
