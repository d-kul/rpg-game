#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Entity.h"
#include "Resource/TileSet.h"

class TileMap : public Entity {
 public:
  void load(TileSet* tileset, const short* tiles,
            float tileSize, sf::Vector2u size);
  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::VertexArray vertices;
  TileSet* tileset = nullptr;
};
