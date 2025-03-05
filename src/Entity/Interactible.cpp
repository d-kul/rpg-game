#include "Interactible.h"

InteractibleEntity::InteractibleEntity(InteractibleManager& manager,
                                       Action* action)
    : action(action), manager(manager) {
  manager.addInteractible(*this);
}

InteractibleEntity::~InteractibleEntity() { manager.removeInteractible(*this); }
