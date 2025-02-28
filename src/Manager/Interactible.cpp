#include "Interactible.h"

#include <SFML/System/Vector2.hpp>
#include <algorithm>

std::unique_ptr<AbstractAction> InteractibleManager::interact(sf::Vector2f position) {
  if (auto it = std::find_if(elements.begin(), elements.end(),
                             [&](InteractibleEntity* i) {
                               return (i->getPosition() - position).length() <
                                      INTERACT_RANGE;
                             });
      it != elements.end()) {
    if ((*it)->action) {
      return (*it)->action->clone();
    }
  }
  return {};
}
