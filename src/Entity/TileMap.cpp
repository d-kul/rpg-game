#include "TileMap.h"

#include <SFML/Graphics/PrimitiveType.hpp>

void TileMap::load(TileSet* tileset, const short* tiles,
                   float tileSize, sf::Vector2u size) {
  auto [width, height] = size;
  auto tileset_size = tileset->getSize();
  auto tileset_tileSize = tileset->tileSize;

  this->tileset = tileset;
  vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  vertices.resize(width * height * 6);

  for (unsigned i = 0; i < height; i++) {
    for (unsigned j = 0; j < width; j++) {
      const short tile = tiles[i * width + j];
      const unsigned tu = tile % (tileset_size.x / tileset_tileSize);
      const unsigned tv = tile / (tileset_size.x / tileset_tileSize);
      sf::Vertex* triangles = &vertices[(i * width + j) * 6];

      triangles[0].position = sf::Vector2f(j * tileSize, i * tileSize);
      triangles[1].position =
          sf::Vector2f((j + 1) * tileSize, i * tileSize);
      triangles[2].position =
          sf::Vector2f(j * tileSize, (i + 1) * tileSize);
      triangles[3].position =
          sf::Vector2f((j + 1) * tileSize, (i + 1) * tileSize);
      triangles[4].position =
          sf::Vector2f(j * tileSize, (i + 1) * tileSize);
      triangles[5].position =
          sf::Vector2f((j + 1) * tileSize, i * tileSize);

      if (tile == -1) {
        triangles[0].color = sf::Color::Transparent;
        triangles[1].color = sf::Color::Transparent;
        triangles[2].color = sf::Color::Transparent;
        triangles[3].color = sf::Color::Transparent;
        triangles[4].color = sf::Color::Transparent;
        triangles[5].color = sf::Color::Transparent;
        continue;
      }

      triangles[0].texCoords =
          sf::Vector2f(tu * tileset_tileSize, tv * tileset_tileSize);
      triangles[1].texCoords =
          sf::Vector2f((tu + 1) * tileset_tileSize, tv * tileset_tileSize);
      triangles[2].texCoords =
          sf::Vector2f(tu * tileset_tileSize, (tv + 1) * tileset_tileSize);
      triangles[3].texCoords = sf::Vector2f((tu + 1) * tileset_tileSize,
                                            (tv + 1) * tileset_tileSize);
      triangles[4].texCoords =
          sf::Vector2f(tu * tileset_tileSize, (tv + 1) * tileset_tileSize);
      triangles[5].texCoords =
          sf::Vector2f((tu + 1) * tileset_tileSize, tv * tileset_tileSize);
    }
  }
}

void TileMap::update(sf::Time dt) {}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // TODO: add view culling perhaps?
  states.transform *= getTransform();
  states.texture = tileset;
  target.draw(vertices, states);
}
