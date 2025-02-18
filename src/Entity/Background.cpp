#include "Background.h"

#include <SFML/System/Vector2.hpp>

#include "Game.h"

Background::Background() : window(Game::getWindow()) {}

Background& Background::setTexture(std::shared_ptr<sf::Texture> texture) {
  this->texture = texture;
  texture->setRepeated(true);
  shape.setSize(sf::Vector2f{window.getSize()});
  shape.setTexture(texture.get());
  shape.setTextureRect(sf::IntRect{{0, 0}, sf::Vector2i{shape.getSize()}});
  shape.setOrigin(shape.getGeometricCenter());
  return *this;
}

Background& Background::setTextureRect(sf::IntRect rect) {
  shape.setTextureRect(rect);
  return *this;
}

Background& Background::setSize(sf::Vector2f size) {
  shape.setSize(sf::Vector2f{window.getSize()});
  return *this;
}

Background& Background::setMoving(bool moving) {
  this->moving = moving;
  return *this;
}

void Background::update(sf::Time dt) {
  if (moving) {
    auto top_left = window.mapPixelToCoords({0, 0});
    shape.setTextureRect(sf::IntRect{sf::FloatRect{top_left, shape.getSize()}});
  }
  setPosition(window.getView().getCenter());
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
}
