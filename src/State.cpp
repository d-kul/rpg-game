#include "State.h"

#include <iostream>

State::State(std::map<std::string, sf::Keyboard::Key>& keybinds)
    : keybinds(keybinds) {
  std::cout << "State::State()" << '\n';
}

State::~State() { std::cout << "State::~State()" << '\n'; }

bool State::isQuit() { return quit; }

void State::startQuit() { quit = true; }

void State::onEvent(const sf::Event& event) {}
