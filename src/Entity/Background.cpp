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
  textureScaling.x = textureSize.x / texture->getSize().x;
  textureScaling.y = textureSize.y / texture->getSize().y;
  texture->setRepeated(repeated);
}

void Background::setSize(sf::Vector2f size) {
  shape.setSize(size);
  shape.setOrigin(shape.getGeometricCenter());
}

void Background::setMoving(bool moving) {
  this->moving = moving;
}

void Background::update(sf::Time dt) {
  if (moving) {
    auto top_left = window.mapPixelToCoords({0, 0});
    top_left.x *= textureScaling.x;
    top_left.y *= textureScaling.y;
    shape.setTextureRect(
        sf::IntRect{sf::FloatRect{top_left,
                                  {shape.getSize().x * textureScaling.x,
                                   shape.getSize().y * textureScaling.y}}});
  }
  setPosition(window.getView().getCenter());
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
}
