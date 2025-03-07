#pragma once

#include "Action.h"
#include "Entity/Interactible.h"

class ActionAction : public Action {
 public:
  ActionAction(InteractibleEntity* holder, Action* action)
      : holder(holder), action(action) {}

  void start() override {
    if (holder) holder->setAction(action);
  }

 private:
  InteractibleEntity* holder;
  Action* action;
  friend class Level;
};
