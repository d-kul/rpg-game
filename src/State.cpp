#include "State.h"

State::State(Game& game) : game(game) {}

State::~State() {}

// State lifetime
void State::exit() {}
void State::enter() {}

// Functionality
void State::update(sf::Time dt) {}
void State::render() {}
