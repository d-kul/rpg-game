#include "State.h"

State::State(keybinds_t& keybinds, sf::RenderWindow& window,
             EventHandler& eventHandler)
    : keybinds(keybinds), window(window), eventHandler(eventHandler) {}

State::~State() {}

// State lifetime
void State::exit() {}
void State::enter() {}

// Functionality
void State::update(sf::Time dt) {}
void State::render() {}
