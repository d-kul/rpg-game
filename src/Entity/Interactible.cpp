#include "Interactible.h"

#include "Game.h"

Interactible::Interactible(sf::Vector2i position, float tileSize)
    : manager(Game::getInteractibleManager()), position(position) {
  sf::Transformable::setPosition(sf::Vector2f{position} * tileSize);
  manager.addInteractible(position, *this);
}

Interactible::~Interactible() { manager.removeInteractible(position); }

void Interactible::update(sf::Time dt) {}

void Interactible::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  states.transform *= getTransform();
  if (drawable) {
    target.draw(*drawable, states);
  }
}
