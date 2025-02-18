#include "State.h"

#include "Game.h"

State::State()
    : keybinds(Game::getKeybinds()),
      window(Game::getWindow()),
      eventHandler(Game::getEventHandler()),
      resourceManager(Game::getResourceManager()),
      audioManager(Game::getAudioManager()) {}

State::~State() {}

// State lifetime
void State::exit() {}
void State::enter() {}

// Functionality
void State::update(sf::Time dt) {}
void State::render() {}
