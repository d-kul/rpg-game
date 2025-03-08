#include "TileMap.h"

#include <SFML/Graphics/PrimitiveType.hpp>

#include "Core/Logger.h"

void TileMap::load(const TileSet& tileset, std::vector<short> tiles,
                   float tileSize, sf::Vector2u size) {
  SDEBUG(" ", &tileset.getTexture(), tiles.size(), tileSize, size.x, size.y);
  this->texture = &tileset.getTexture();
  this->tileSize = tileSize;
  this->size = size;

  auto [width, height] = size;
  auto tileset_tile_size = tileset.tileSize;
  vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  vertices.clear();
  vertices.resize(width * height * 6);

  for (unsigned i = 0; i < height; i++) {
    for (unsigned j = 0; j < width; j++) {
      const short tile = tiles[i * width + j];
      const unsigned tu = tile % tileset.getSize().x;
      const unsigned tv = tile / tileset.getSize().x;
      sf::Vertex* triangles = &vertices[(i * width + j) * 6];

      triangles[0].position = sf::Vector2f(j * tileSize, i * tileSize);
      triangles[1].position = sf::Vector2f((j + 1) * tileSize, i * tileSize);
      triangles[2].position = sf::Vector2f(j * tileSize, (i + 1) * tileSize);
      triangles[3].position =
          sf::Vector2f((j + 1) * tileSize, (i + 1) * tileSize);
      triangles[4].position = sf::Vector2f(j * tileSize, (i + 1) * tileSize);
      triangles[5].position = sf::Vector2f((j + 1) * tileSize, i * tileSize);

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
          sf::Vector2f(tu * tileset_tile_size.x, tv * tileset_tile_size.y);
      triangles[1].texCoords = sf::Vector2f((tu + 1) * tileset_tile_size.x,
                                            tv * tileset_tile_size.y);
      triangles[2].texCoords = sf::Vector2f(tu * tileset_tile_size.x,
                                            (tv + 1) * tileset_tile_size.y);
      triangles[3].texCoords = sf::Vector2f((tu + 1) * tileset_tile_size.x,
                                            (tv + 1) * tileset_tile_size.y);
      triangles[4].texCoords = sf::Vector2f(tu * tileset_tile_size.x,
                                            (tv + 1) * tileset_tile_size.y);
      triangles[5].texCoords = sf::Vector2f((tu + 1) * tileset_tile_size.x,
                                            tv * tileset_tile_size.y);
    }
  }
}

void TileMap::clear() {
  vertices.clear();
  texture = nullptr;
}

sf::Vector2u TileMap::getSize() { return size; }

void TileMap::update(sf::Time dt) {}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // TODO(des): add view culling perhaps?
  states.transform *= getTransform();
  states.texture = texture;
  target.draw(vertices, states);
}
