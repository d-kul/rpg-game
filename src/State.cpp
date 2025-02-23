#include "State.h"

#include "Game.h"

State::State()
    : keybinds(Game::getKeybinds()),
      window(Game::getWindow()),
      windowSize(Game::getWindowSize()),
      eventManager(Game::getEventManager()),
      resourceManager(Game::getResourceManager()),
      audioManager(Game::getAudioManager()),
      uiManager(Game::getUIManager()) {}

State::~State() {}

// State lifetime
void State::exit() {}
void State::enter() {}

// Functionality
void State::update(sf::Time dt) {}
void State::render() {}
