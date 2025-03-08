#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Entity.h"
#include "Resource/TileSet.h"

class TileMap : public Entity {
 public:
  void load(const TileSet& tileset, std::vector<short> tiles, float tileSize,
            sf::Vector2u size);

  void clear();

  sf::Vector2u getSize();

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  float tileSize;

 private:
  sf::VertexArray vertices;
  const sf::Texture* texture;
  sf::Vector2u size;
};
