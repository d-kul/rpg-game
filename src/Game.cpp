#include "Game.h"

#include <SFML/Window.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <optional>

#include "Config.h"
#include "State/MainMenuState.h"

// Singleton access
Game& Game::instance() { return instance_; }

// Singleton member access
sf::RenderWindow& Game::getWindow() { return instance_.window; }

Game::states_t& Game::getStates() { return instance_.states; }

Game::keybinds_t& Game::getKeybinds() { return instance_.keybinds; }

// Initialization
void Game::initWindow() {
  (void)sf::VideoMode::getFullscreenModes();
  // Load options from "window.ini" file
  Config config{"config/window.ini"};
  std::string title = config.get("title").value_or("something cool");
  sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
  if (auto value = config.get<unsigned>("width")) video_mode.size.x = *value;
  if (auto value = config.get<unsigned>("height")) video_mode.size.y = *value;
  fullscreen = config.get<bool>("fullscreen").value_or(false);
  unsigned framerate_limit =
      config.get<unsigned>("framerate_limit").value_or(144);
  bool vsync_enabled = config.get<bool>("vsync_enabled").value_or(false);
  unsigned antialiasing_level =
      config.get<unsigned>("antialiasing_level").value_or(0);

  context_settings.antiAliasingLevel = antialiasing_level;
  // context_settings.attributeFlags = sf::ContextSettings::Debug;

  window.create(
      video_mode, title,
      fullscreen ? sf::Style::None : sf::Style::Titlebar | sf::Style::Close,
      fullscreen ? sf::State::Fullscreen : sf::State::Windowed,
      context_settings);
  window.setFramerateLimit(framerate_limit);
  window.setVerticalSyncEnabled(vsync_enabled);
}

void Game::initKeybinds() {
  keybinds = default_keybinds;
  Config config{"config/keybinds.ini"};
  for (auto& entry : config) {
    if (keybinds.count(entry.first) && key_index.count(entry.second)) {
      keybinds.at(entry.first) = key_index.at(entry.second);
    }
  }
}

void Game::initStates() { states.emplace(new MainMenuState{}); }

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
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
    if (!states.empty()) {
      states.top()->onEvent(*event);
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
    update(clock.restart());
    render();
  }
}
