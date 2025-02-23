#include "Grid.h"

#include <SFML/System/Vector2.hpp>
#include <cmath>

#include "Core/Logger.h"

Grid::Grid(sf::Vector2f spacing, sf::Vector2f offset, sf::Color color)
    : spacing(spacing), offset(offset), color(color) {
  vertices.setPrimitiveType(sf::PrimitiveType::Lines);
}

void Grid::setSpacing(sf::Vector2f spacing) { this->spacing = spacing; }

void Grid::setOffset(sf::Vector2f offset) { this->offset = offset; }

void Grid::setColor(sf::Color color) { this->color = color; }

void Grid::setSize(sf::Vector2f size) {
  assert(spacing.x != 0 && spacing.y != 0);
  this->size = size;
  auto half_grid_size_f = size.componentWiseDiv(spacing) / 2.f;
  auto half_grid_size = sf::Vector2i(std::floor(half_grid_size_f.x),
                                     std::floor(half_grid_size_f.y));
  auto grid_size = half_grid_size * 2;
  grid_size += {1, 1};

  vertices.resize((grid_size.x + 2) * 2 + (grid_size.y + 2) * 2);
  for (int i = 0, x = -half_grid_size.x - 1; x <= half_grid_size.x + 1;
       i += 2, x++) {
    sf::Vertex* vertices = &this->vertices[i];
    vertices[0].position = {x * spacing.x, (-size.y / 2.f) - 1 * spacing.y};
    vertices[1].position = {x * spacing.x, (+size.y / 2.f) + 1 * spacing.y};
    vertices[0].color = color;
    vertices[1].color = color;
  }

  for (int y = -half_grid_size.y - 1, i = (grid_size.x + 2) * 2;
       y <= half_grid_size.y + 1; i += 2, y++) {
    sf::Vertex* vertices = &this->vertices[i];
    vertices[0].position = {(-size.x / 2.f) - 1 * spacing.x, y * spacing.y};
    vertices[1].position = {(+size.x / 2.f) + 1 * spacing.x, y * spacing.y};
    vertices[0].color = color;
    vertices[1].color = color;
  }
}

void Grid::setPosition(sf::Vector2f position) {
  centerPosition = position;
  centerPosition = {std::floor(centerPosition.x / spacing.x) * spacing.x,
                    std::floor(centerPosition.y / spacing.y) * spacing.y};
  centerPosition += {std::floor(offset.x / spacing.x) * spacing.x,
                     std::floor(offset.y / spacing.y) * spacing.y};
}

void Grid::setView(const sf::View& view) {
  setSize(view.getSize());
  setPosition(view.getCenter());
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform.translate(centerPosition);
  target.draw(vertices, states);
}
