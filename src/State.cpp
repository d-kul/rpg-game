#include "State.h"

#include "Game.h"

State::State()
    : keybinds(Game::getKeybinds()),
      window(Game::getWindow()),
      registry(Game::getRegistry()) {}

State::~State() {
  for (auto entity : entities) {
    registry.destroy(entity);
  }
}

// State lifetime
void State::exit() {}
void State::enter() {}

// Functionality
void State::update() {}

entt::entity State::create_entity() {
  return entities.emplace_back(registry.create());
}
