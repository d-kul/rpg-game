#include "Interactible.h"

#include <algorithm>

#include "Core/Logger.h"

void InteractibleManager::addInteractible(InteractibleEntity& interactible) {
  DEBUG("added interactible ", &interactible);
  elements.push_back(&interactible);
}

void InteractibleManager::removeInteractible(InteractibleEntity& interactible) {
  DEBUG("removed interactible ", &interactible);
  elements.erase(std::find(elements.begin(), elements.end(), &interactible));
}

Action* InteractibleManager::interact(sf::Vector2f position) {
  auto it = std::find_if(
      elements.begin(), elements.end(), [&](InteractibleEntity* i) {
        return (i->getPosition() - position).length() < INTERACT_RANGE;
      });
  if (it != elements.end()) {
    if ((*it)->action) {
      return (*it)->action;
    }
  }
  return nullptr;
}
