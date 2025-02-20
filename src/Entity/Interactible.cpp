#include "Interactible.h"

#include "Game.h"

Interactible::Interactible(sf::Vector2i position)
    : manager(Game::getInteractibleManager()) {
  setPosition(position);
}

void Interactible::setPosition(sf::Vector2i position) {
  sf::Transformable::setPosition(sf::Vector2f{position} * tileSize);
  manager.addInteractible(position, this);
}

void Interactible::update(sf::Time dt) {}

void Interactible::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  states.transform *= getTransform();
  if (drawable) {
    target.draw(*drawable, states);
  }
}
