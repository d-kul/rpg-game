#include "State.h"

#include <iostream>

State::State() { std::cout << "State::State()" << '\n'; }

State::~State() { std::cout << "State::~State()" << '\n'; }

bool State::isQuit() { return quit; }

void State::startQuit() { quit = true; }

void State::onEvent(const sf::Event& event) {}
