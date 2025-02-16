#include "State.h"

State::State(const State& other)
    : keybinds(other.keybinds), window(other.window) {}

State::State(keybinds_t& keybinds, sf::RenderWindow& window)
    : keybinds(keybinds), window(window) {}

State::~State() {}

// State lifetime
void State::exit() {}
void State::enter() {}

// Functionality
void State::update(sf::Time dt) {}
void State::render() {}
