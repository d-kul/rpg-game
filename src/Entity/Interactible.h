#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Action.h"
#include "Entity.h"

class InteractibleEntity : public virtual Entity {
 public:
  InteractibleEntity(std::unique_ptr<AbstractAction> action = {});
  ~InteractibleEntity();

  void setAction(std::unique_ptr<AbstractAction> action) {
    this->action = std::move(action);
  }

 protected:
  std::unique_ptr<AbstractAction> action;

  friend class InteractibleManager;
};
