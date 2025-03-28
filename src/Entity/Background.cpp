#include "Background.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

Background::Background() { shape.setFillColor(sf::Color::Transparent); }

sf::Texture* Background::getTexture() { return texture; }

void Background::setTexture(sf::Texture* texture) {
  setTexture(texture, sf::Vector2f{texture->getSize()});
}

void Background::setTexture(sf::Texture* texture, sf::Vector2f textureSize) {
  this->texture = texture;
  shape.setTexture(texture);
  shape.setFillColor(sf::Color::White);
  setTextureSize(textureSize);
}

void Background::unsetTexture() {
  this->texture = nullptr;
  shape.setTexture(nullptr);
  shape.setFillColor(sf::Color::Transparent);
  textureScaling = {1.f, 1.f};
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
  auto top_left =
      moving ? view.getCenter() - view.getSize() / 2.f : sf::Vector2f{0, 0};
  top_left = top_left.componentWiseDiv(textureScaling);
  shape.setTextureRect(sf::IntRect{sf::FloatRect{
      top_left, shape.getSize().componentWiseDiv(textureScaling)}});
  setPosition(view.getCenter());
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
}
