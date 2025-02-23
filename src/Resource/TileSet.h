#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

class TileSet : public sf::Texture {
 public:
  TileSet(const std::filesystem::path& filename, unsigned tileSize)
      : sf::Texture(filename), tileSize(tileSize) {
    assert(sf::Texture::getSize().x % tileSize == 0 &&
           sf::Texture::getSize().y % tileSize == 0 &&
           "tileset dimensions should be divisible by tile size");
  }

  sf::IntRect getTileRect(int tile) const;

  unsigned tileSize;
};
