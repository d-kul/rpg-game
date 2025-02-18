#include "TileSet.h"

sf::IntRect TileSet::getTileRect(int tile) const {
  int w = getSize().x / tileSize.x;
  return sf::IntRect{{(tile % w) * tileSize.x, (tile / w) * tileSize.y},
                     tileSize};
}
