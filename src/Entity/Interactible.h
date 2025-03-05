#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Action.h"
#include "Entity.h"

class InteractibleEntity;

#include "Manager/Interactible.h"

class InteractibleEntity : public virtual Entity {
 public:
  InteractibleEntity(InteractibleManager& manager, Action* action = nullptr);
  ~InteractibleEntity();

  void setAction(Action* action) { this->action = action; }

 protected:
  Action* action;
  InteractibleManager& manager;

  friend class InteractibleManager;
};
