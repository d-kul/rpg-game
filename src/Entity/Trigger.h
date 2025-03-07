#pragma once

#include "Action.h"

class Trigger;

#include "Entity/Interactible.h"
#include "Manager/Interactible.h"

class Trigger : public InteractibleEntity {
 public:
  Trigger(InteractibleManager& manager, Action* action = nullptr);
  ~Trigger();
};
