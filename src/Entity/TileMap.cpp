#include "TileMap.h"

#include <SFML/Graphics/PrimitiveType.hpp>

#include "Core/Logger.h"

void TileMap::load(std::shared_ptr<TileSet> tileset, const unsigned* tiles,
                   sf::Vector2i tileSize, sf::Vector2u size) {
  auto [width, height] = size;
  auto tileset_size = tileset->getSize();
  auto tileset_tileSize = tileset->tileSize;

  this->tileset = tileset;
  vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  vertices.resize(width * height * 6);

  for (unsigned i = 0; i < height; i++) {
    for (unsigned j = 0; j < width; j++) {
      const unsigned tile = tiles[i * width + j];
      const unsigned tu = tile % (tileset_size.x / tileset_tileSize.x);
      const unsigned tv = tile / (tileset_size.x / tileset_tileSize.x);
      sf::Vertex* triangles = &vertices[(i * width + j) * 6];

      triangles[0].position = sf::Vector2f(j * tileSize.x, i * tileSize.y);
      triangles[1].position =
          sf::Vector2f((j + 1) * tileSize.x, i * tileSize.y);
      triangles[2].position =
          sf::Vector2f(j * tileSize.x, (i + 1) * tileSize.y);
      triangles[3].position =
          sf::Vector2f((j + 1) * tileSize.x, (i + 1) * tileSize.y);
      triangles[4].position =
          sf::Vector2f(j * tileSize.x, (i + 1) * tileSize.y);
      triangles[5].position =
          sf::Vector2f((j + 1) * tileSize.x, i * tileSize.y);

      triangles[0].texCoords =
          sf::Vector2f(tu * tileset_tileSize.x, tv * tileset_tileSize.y);
      triangles[1].texCoords =
          sf::Vector2f((tu + 1) * tileset_tileSize.x, tv * tileset_tileSize.y);
      triangles[2].texCoords =
          sf::Vector2f(tu * tileset_tileSize.x, (tv + 1) * tileset_tileSize.y);
      triangles[3].texCoords = sf::Vector2f((tu + 1) * tileset_tileSize.x,
                                            (tv + 1) * tileset_tileSize.y);
      triangles[4].texCoords =
          sf::Vector2f(tu * tileset_tileSize.x, (tv + 1) * tileset_tileSize.y);
      triangles[5].texCoords =
          sf::Vector2f((tu + 1) * tileset_tileSize.x, tv * tileset_tileSize.y);
    }
  }
}

void TileMap::update(sf::Time dt) {}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // TODO: add view culling perhaps?
  states.transform *= getTransform();
  states.texture = tileset.get();
  target.draw(vertices, states);
}
