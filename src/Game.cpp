#include "Game.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <entt/entity/fwd.hpp>
#include <optional>

#include "Config.h"
#include "State/MainMenuState.h"

// Singleton access
Game& Game::instance() { return instance_; }

// Singleton member access
sf::RenderWindow& Game::getWindow() { return instance_.window; }

keybinds_t& Game::getKeybinds() { return instance_.keybinds; }

entt::registry& Game::getRegistry() { return instance_.registry; }

void Game::release(retained_t::iterator it) { instance_.retained.erase(it); }

// Initialization
void Game::initWindow() {
  // Load options from "window.ini" file
  Config config{"config/window.ini"};
  std::string title = config.get("title").value_or("something cool");
  sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
  fullscreen = config.get<bool>("fullscreen").value_or(false);
  if (auto width = config.get<unsigned>("width"),
      height = config.get<unsigned>("height");
      width && height) {
    if (fullscreen) {
      auto& modes = sf::VideoMode::getFullscreenModes();
      auto it = std::find_if(
          modes.begin(), modes.end(), [&](const sf::VideoMode& mode) {
            return mode.size.x == width && mode.size.y == height;
          });
      if (it != modes.end()) {
        video_mode = *it;
      }
    } else {
      video_mode.size.x = *width;
      video_mode.size.y = *height;
    }
  }
  unsigned framerate_limit =
      config.get<unsigned>("framerate_limit").value_or(144);
  bool vsync_enabled = config.get<bool>("vsync_enabled").value_or(false);
  unsigned antialiasing_level =
      config.get<unsigned>("antialiasing_level").value_or(0);

  auto screen_middle =
      (sf::VideoMode::getDesktopMode().size - video_mode.size) / 2u;

  context_settings.antiAliasingLevel = antialiasing_level;
  window.create(
      video_mode, title,
      fullscreen ? sf::Style::None : sf::Style::Titlebar | sf::Style::Close,
      fullscreen ? sf::State::Fullscreen : sf::State::Windowed,
      context_settings);
  window.setFramerateLimit(framerate_limit);
  window.setVerticalSyncEnabled(vsync_enabled);
  window.setPosition(sf::Vector2i{screen_middle});
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
  registry.sort<DrawableComponent>(std::less<DrawableComponent>{});
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
  retained.clear();
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
    registry.sort<DrawableComponent>(std::less<DrawableComponent>{});
  }
}

void Game::render() {
  window.clear();
  state->render();
  render_system.render(registry, window);
  window.display();
}

void Game::run() {
  while (window.isOpen()) {
    update(clock.restart());
    render();
  }
}
