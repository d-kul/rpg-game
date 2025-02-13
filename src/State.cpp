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
void State::render() {}

void State::handleEvent(const sf::Event& event) {}

std::unique_ptr<State> State::getNext() { return std::move(next_state); }

entt::entity State::create() {
  return entities.emplace_back(registry.create());
}
