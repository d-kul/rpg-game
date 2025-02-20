#include "Interactible.h"

#include <SFML/System/Vector2.hpp>

void InteractibleManager::addInteractible(sf::Vector2i position,
                                          Interactible& i) {
  interactibles.insert({position, &i});
}

void InteractibleManager::interact(sf::Vector2i position) {
  if (auto it = interactibles.find(position); it != interactibles.end()) {
    if (it->second->action) {
      it->second->action();
    }
  }
}
