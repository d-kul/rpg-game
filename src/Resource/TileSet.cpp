#include "TileSet.h"

sf::IntRect TileSet::getTileRect(sf::Vector2i coords) {
  return sf::IntRect{{coords.x * tileSize.x, coords.y * tileSize.y}, tileSize};
}
