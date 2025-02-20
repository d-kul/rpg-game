#include "TileSet.h"

sf::IntRect TileSet::getTileRect(int tile) const {
  int w = getSize().x / tileSize;
  return sf::IntRect{
      sf::Vector2i{sf::Vector2u{(tile % w) * tileSize, (tile / w) * tileSize}},
      sf::Vector2i{sf::Vector2u{tileSize, tileSize}}};
}
