#include "Manager/Interactible.h"

InteractibleEntity::InteractibleEntity(InteractibleManager& manager,
                                       Action* action)
    : action(action), manager(manager) {
  manager.add(*this);
}

InteractibleEntity::~InteractibleEntity() { manager.remove(*this); }
