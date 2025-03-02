#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "Resource/AbstractSpriteSheet.h"

class TileSet : public AbstractSpriteSheet {
 public:
  TileSet() = default;

  TileSet(const sf::Texture& texture, sf::Vector2u tileSize)
      : AbstractSpriteSheet(texture), tileSize(tileSize) {}

  TileSet(const sf::Texture& texture, unsigned tileSize)
      : TileSet(texture, {tileSize, tileSize}) {}

  sf::IntRect getRect(int index) const override {
    int width = texture->getSize().x / tileSize.x;
    return {sf::Vector2i{index % width, index / width}.componentWiseMul(
                sf::Vector2i{tileSize}),
            sf::Vector2i{tileSize}};
  }

  unsigned getRectAmount() const override {
    auto size = getSize();
    return size.x * size.y;
  }

  sf::Vector2u getSize() const {
    return texture->getSize().componentWiseDiv(tileSize);
  }

  sf::Vector2u tileSize;
};
