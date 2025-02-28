#include "Interactible.h"

#include "Game.h"

InteractibleEntity::InteractibleEntity(std::unique_ptr<AbstractAction> action)
    : action(std::move(action)) {
  Game::getInteractibleManager().elements.insert(this);
}

InteractibleEntity::~InteractibleEntity() {
  Game::getInteractibleManager().elements.erase(this);
}
