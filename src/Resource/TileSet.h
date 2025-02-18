#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "Core/Logger.h"

class TileSet : public sf::Texture {
 public:
  template <typename... Args>
  TileSet(sf::Vector2i tileSize, Args&&... args)
      : sf::Texture(std::forward<Args>(args)...), tileSize(tileSize) {
    SDEBUG(" ", sf::Texture::getSize().x, tileSize.x, sf::Texture::getSize().y,
           tileSize.y);
    assert(sf::Texture::getSize().x % tileSize.x == 0 &&
           sf::Texture::getSize().y % tileSize.y == 0 &&
           "tileset dimensions should be divisible by tile size");
  }

  sf::IntRect getTileRect(sf::Vector2i coords);

  sf::Vector2i tileSize;
};
