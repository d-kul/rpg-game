#include "Game.h"

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <optional>

#include "Core/Config.h"
#include "State/MainMenu.h"

// Singleton interface
Game& Game::getInstance() { return instance; }

sf::RenderWindow& Game::getWindow() { return instance.window; }
sf::Vector2u& Game::getWindowSize() { return instance.windowSize; }
keybinds_t& Game::getKeybinds() { return instance.keybinds; }

EventManager& Game::getEventManager() { return instance.eventManager; }
ResourceManager& Game::getResourceManager() { return instance.resourceManager; }
AudioManager& Game::getAudioManager() { return instance.audioManager; }
InteractibleManager& Game::getInteractibleManager() {
  return instance.interactibleManager;
}
ColliderManager& Game::getColliderManager() { return instance.colliderManager; }

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
    video_mode.size.x = *width;
    video_mode.size.y = *height;
    if (fullscreen && !video_mode.isValid()) {
      video_mode = sf::VideoMode::getDesktopMode();
    }
  }
  auto framerate_limit = config.get<unsigned>("framerate_limit");
  bool vsync_enabled = config.get<bool>("vsync_enabled").value_or(false);
  unsigned antialiasing_level =
      config.get<unsigned>("antialiasing_level").value_or(0);

  auto screen_middle =
      (sf::VideoMode::getDesktopMode().size - video_mode.size) / 2u;

  contextSettings.antiAliasingLevel = antialiasing_level;
  window.create(
      video_mode, title,
      fullscreen ? sf::Style::None : sf::Style::Titlebar | sf::Style::Close,
      fullscreen ? sf::State::Fullscreen : sf::State::Windowed,
      contextSettings);
  if (framerate_limit) {
    window.setFramerateLimit(*framerate_limit);
  } else {
    window.setVerticalSyncEnabled(vsync_enabled);
  }
  if (!fullscreen) window.setPosition(sf::Vector2i{screen_middle});
  windowSize = video_mode.size;
  window.setView(
      sf::View{sf::Vector2f{windowSize} / 2.f, sf::Vector2f{windowSize}});
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
  state = new MainMenuState{};
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
  delete state;
}

// Functionality
void Game::handleEvents() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
    event->visit(
        [&](auto&& event) { eventManager.sink<decltype(event)>()(event); });
  }
}

void Game::update(sf::Time dt) {
  handleEvents();
  state->update(dt);
  audioManager.clearStoppedSounds();
  if (auto next_state = state->next_state) {
    state->exit();
    // TODO:
    // eventManager.clearListeners();
    // // nothing for resourceManager...
    // audioManager.clearAllSounds();
    // interactibleManager.clearInteractibles();
    // colliderManager.clearColliders();
    delete state;
    state = next_state;
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
