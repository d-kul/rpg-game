#include "Game.h"

#include <SFML/Window.hpp>
#include <optional>

#include "Config.h"
#include "State/MainMenuState.h"

// Initialization
void Game::initWindow() {
  std::string title = "something cool";
  sf::VideoMode video_mode({800u, 600u});
  unsigned framerate_limit = 144;
  bool vsync_enabled = false;

  // Load options from "window.ini" file
  Config config{"config/window.ini"};
  if (auto value = config.get("title")) title = *value;
  if (auto value = config.get<unsigned>("width")) video_mode.size.x = *value;
  if (auto value = config.get<unsigned>("height")) video_mode.size.y = *value;
  if (auto value = config.get<unsigned>("framerate_limit"))
    framerate_limit = *value;
  if (auto value = config.get<unsigned>("vsync_enabled"))
    vsync_enabled = *value;

  window.create(video_mode, title);
  window.setFramerateLimit(framerate_limit);
  window.setVerticalSyncEnabled(vsync_enabled);
}

void Game::initKeybinds() {
  Config config{"config/keybinds.ini"};
  for (auto& entry : config) {
    if (keys.count(entry.second))
      keybinds.emplace(entry.first, keys.at(entry.second));
  }
}

void Game::initStates() {
  states.emplace(new MainMenuState{
      keybinds, static_cast<sf::Vector2f>(window.getSize()), states});
}

// Constructors, destructor
Game::Game() {
  initWindow();
  initKeybinds();
  initStates();
}

Game::~Game() {}

// Functionality
void Game::pollEvents() {
  while (const std::optional event = window.pollEvent()) {
    if (!states.empty()) {
      states.top()->onEvent(*event);
    }
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
  }
}

void Game::update(sf::Time dt) {
  pollEvents();
  if (!states.empty()) {
    states.top()->update(dt);
    if (states.top()->isQuit()) {
      states.top()->endState();
      states.pop();
    }
  } else {
    window.close();
  }
}

void Game::render() {
  window.clear();
  if (!states.empty()) {
    states.top()->render(window);
  }
  window.display();
}

void Game::run() {
  while (window.isOpen()) {
    sf::Time dt = clock.restart();
    update(dt);
    render();
  }
}
