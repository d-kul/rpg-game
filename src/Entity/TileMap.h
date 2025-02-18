#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <memory>

#include "Entity.h"
#include "Resource/TileSet.h"

class TileMap : public Entity {
 public:
  void load(std::shared_ptr<TileSet> tileset, const unsigned* tiles,
            sf::Vector2i tileSize, sf::Vector2u size);
  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::VertexArray vertices;
  std::shared_ptr<TileSet> tileset;
};
