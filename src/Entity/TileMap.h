#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Entity.h"
#include "Resource/TileSet.h"

class TileMap : public Entity {
 public:
  void load(TileSet* tileset, std::vector<short> tiles, float tileSize,
            sf::Vector2u size);

  sf::Vector2u getSize();

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  float tileSize;

 private:
  sf::VertexArray vertices;
  TileSet* tileset = nullptr;
  std::vector<short> tiles;
  sf::Vector2u size;
};
