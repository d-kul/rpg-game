#include "Interactible.h"

#include <algorithm>

#include "Core/Logger.h"

void InteractibleManager::add(InteractibleEntity& interactible) {
  DEBUG("added interactible ", &interactible);
  interactibles.push_back(&interactible);
}

void InteractibleManager::remove(InteractibleEntity& interactible) {
  DEBUG("removed interactible ", &interactible);
  interactibles.erase(
      std::find(interactibles.begin(), interactibles.end(), &interactible));
}

void InteractibleManager::addTrigger(InteractibleEntity& trigger) {
  DEBUG("added trigger ", &trigger);
  triggers.insert(&trigger);
}

void InteractibleManager::removeTrigger(InteractibleEntity& trigger) {
  DEBUG("removed trigger ", &trigger);
  triggers.erase(&trigger);
}

Action* InteractibleManager::interact(sf::Vector2f position) {
  auto it = std::find_if(
      interactibles.begin(), interactibles.end(), [&](InteractibleEntity* i) {
        return triggers.count(i) == 0 &&
               (i->getPosition() - position).length() < INTERACT_RANGE;
      });
  if (it != interactibles.end()) {
    if ((*it)->action) {
      return (*it)->action;
    }
  }
  return nullptr;
}

Action* InteractibleManager::trigger(sf::Vector2f position) {
  auto it = std::find_if(
      triggers.begin(), triggers.end(), [&](InteractibleEntity* i) {
        return (i->getPosition() - position).length() < INTERACT_RANGE;
      });
  if (it != triggers.end()) {
    if ((*it)->action) {
      return (*it)->action;
    }
  }
  return nullptr;
}
