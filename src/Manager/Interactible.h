#pragma once

#include <SFML/System/Vector2.hpp>
#include <unordered_set>
#include <vector>

class InteractibleManager;

#include "Entity/Interactible.h"

class InteractibleManager {
 public:
  static constexpr float INTERACT_RANGE = 3.f;

 public:
  void add(InteractibleEntity& interactible);
  void remove(InteractibleEntity& interactible);
  void addTrigger(InteractibleEntity& trigger);
  void removeTrigger(InteractibleEntity& trigger);
  Action* interact(sf::Vector2f position);
  Action* trigger(sf::Vector2f position);

 private:
  std::vector<InteractibleEntity*> interactibles;
  std::unordered_set<InteractibleEntity*> triggers;
};
