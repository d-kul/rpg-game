#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

class TileSet : public sf::Texture {
 public:
  template <typename... Args>
  TileSet(sf::Vector2i tileSize, Args&&... args)
      : sf::Texture(std::forward<Args>(args)...), tileSize(tileSize) {
    assert(sf::Texture::getSize().x % tileSize.x == 0 &&
           sf::Texture::getSize().y % tileSize.y == 0 &&
           "tileset dimensions should be divisible by tile size");
  }

  sf::IntRect getTileRect(int tile) const;

  sf::Vector2i tileSize;
};
