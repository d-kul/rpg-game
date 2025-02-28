#pragma once

#include <SFML/System/Vector2.hpp>
#include <set>

#include "Entity/Interactible.h"

class InteractibleManager {
 public:
  using elements_t = std::set<InteractibleEntity*>;
  static constexpr float INTERACT_RANGE = 3.f;

 public:
  std::unique_ptr<AbstractAction> interact(sf::Vector2f position);

 private:
  elements_t elements;

  friend class InteractibleEntity;
};
