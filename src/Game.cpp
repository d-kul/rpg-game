#include "Game.h"

#include <SFML/Window.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <entt/entity/fwd.hpp>
#include <optional>

#include "Config.h"
#include "State/GameState.h"
#include "State/MainMenuState.h"

// Singleton access
Game& Game::instance() { return instance_; }

// Singleton member access
sf::RenderWindow& Game::getWindow() { return instance_.window; }

keybinds_t& Game::getKeybinds() { return instance_.keybinds; }

entt::registry& Game::getRegistry() { return instance_.registry; }

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

void Game::initRegistry() {}

void Game::initState() {
  state = std::make_unique<MainMenuState>();
  state->enter();
}

// Constructors, destructor
Game::Game() {
  initWindow();
  initKeybinds();
  initRegistry();
  initState();
}

Game::~Game() { 
  state->exit();
  state.reset();
  registry.clear(); 
}

// Functionality
void Game::handleEvents() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
    state->handleEvent(*event);
    event_handler_system.handleEvent(registry, *event);
  }
}

void Game::update(sf::Time dt) {
  handleEvents();
  player_system.update(registry, dt);
  movement_system.update(registry, dt);
  sound_system.clearStopped(registry);
  if (auto next_state = state->getNext()) {
    state->exit();
    sound_system.clearAll(registry);
    state = std::move(next_state);
    state->enter();
  }
}

void Game::render() {
  window.clear();
  state->render();
  window.display();
}

void Game::run() {
  while (window.isOpen()) {
    update(clock.restart());
    render();
  }
}
