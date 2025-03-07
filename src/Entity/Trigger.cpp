#include "Trigger.h"

Trigger::Trigger(InteractibleManager& manager, Action* action)
    : InteractibleEntity(manager, action) {
  manager.addTrigger(*this);
}

Trigger::~Trigger() { manager.removeTrigger(*this); }
