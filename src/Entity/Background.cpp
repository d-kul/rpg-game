#include "Background.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "Game.h"

Background::Background(bool moving)
    : window(Game::getWindow()), moving(moving) {}

void Background::setTexture(sf::Texture* texture, bool repeated) {
  setTexture(texture, sf::Vector2f{texture->getSize()}, repeated);
}

void Background::setTexture(sf::Texture* texture, sf::Vector2f textureSize,
                            bool repeated) {
  shape.setTexture(texture);
  textureScaling =
      textureSize.componentWiseDiv(sf::Vector2f{texture->getSize()});
  texture->setRepeated(repeated);
}

void Background::setSize(sf::Vector2f size) {
  shape.setSize(size);
  shape.setOrigin(shape.getGeometricCenter());
}

void Background::setMoving(bool moving) { this->moving = moving; }

void Background::update(sf::Time dt) {
  if (moving) {
    auto top_left = window.mapPixelToCoords({0, 0});
    top_left = top_left.componentWiseMul(textureScaling);
    shape.setTextureRect(sf::IntRect{sf::FloatRect{
        top_left, shape.getSize().componentWiseMul(textureScaling)}});
  }
  setPosition(window.getView().getCenter());
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
}
