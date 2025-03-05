#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

class InteractibleManager;

#include "Entity/Interactible.h"

class InteractibleManager {
 public:
  static constexpr float INTERACT_RANGE = 3.f;

 public:
  void addInteractible(InteractibleEntity& interactible);
  void removeInteractible(InteractibleEntity& interactible);
  Action* interact(sf::Vector2f position);

 private:
  std::vector<InteractibleEntity*> elements;
};
