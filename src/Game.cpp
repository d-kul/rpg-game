#include "Game.h"

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <optional>

#include "Core/Config.h"
#include "State/MainMenuState.h"
#include "UI/Frame.h"
#include "UI/Text.h"

// Initialization
void Game::initWindowConfig() {
  // Load options from "window.ini" file
  Config config{"config/window.ini"};

#ifdef NDEBUG
  videoMode = sf::VideoMode::getDesktopMode();
#else
  title = config.get("title").value_or("something cool");
  videoMode = sf::VideoMode::getDesktopMode();
  if (auto width = config.get<unsigned>("width"),
      height = config.get<unsigned>("height");
      width && height) {
    videoMode.size.x = *width;
    videoMode.size.y = *height;
    if (fullscreen && !videoMode.isValid()) {
      videoMode = sf::VideoMode::getDesktopMode();
    }
  }
#endif
  fullscreen = config.get<bool>("fullscreen").value_or(false);
  createWindow();

  bool vsyncEnabled = config.get<bool>("vsync_enabled").value_or(false);
  if (vsyncEnabled) {
    window.setVerticalSyncEnabled(vsyncEnabled);
  } else {
    window.setFramerateLimit(
        config.get<unsigned>("framerate_limit").value_or(60));
  }
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

void Game::initManagers() {
  uiManager.init(videoMode.size);
  auto main_frame = std::make_unique<Frame>();
  loading_frame = main_frame.get();
  main_frame->shape.setSize(sf::Vector2f{videoMode.size});
  main_frame->shape.setFillColor(sf::Color(0, 0, 0, 128));
  auto font =
      resourceManager.retain<sf::Font>("resources/fonts/DroidSansMono.ttf");
  auto text = std::make_unique<Text>(*font, "Loading...");
  loading_text = text.get();
  text->text.setFillColor(sf::Color::White);
  text->text.setOrigin(text->text.getGlobalBounds().getCenter());
  text->setPosition(main_frame->shape.getGeometricCenter());
  main_frame->addChild(std::move(text));
  uiManager.states["loading"] = std::move(main_frame);
}

void Game::initState() {
  state = new MainMenuState{*this};
  state->enter();
}

void Game::setFullscreen(bool fullscreen) {
  this->fullscreen = fullscreen;
  createWindow();
}

void Game::updateSize() {
  window.setSize(videoMode.size);
  auto size = sf::Vector2f{videoMode.size};
  if (!fullscreen)
    window.setPosition(
        sf::Vector2i{sf::VideoMode::getDesktopMode().size - videoMode.size} /
        2);
  window.setView(sf::View{size / 2.f, size});
  uiManager.setView(window.getView());
  loading_frame->shape.setSize(size);
  loading_text->setPosition(size / 2.f);
}

void Game::createWindow() {
  window.create(
      videoMode, title,
      fullscreen ? sf::Style::None : sf::Style::Titlebar | sf::Style::Close,
      fullscreen ? sf::State::Fullscreen : sf::State::Windowed,
      contextSettings);
  if (!fullscreen)
    window.setPosition(
        sf::Vector2i{sf::VideoMode::getDesktopMode().size - videoMode.size} /
        2);
  window.setView(sf::View{sf::Vector2f{videoMode.size} / 2.f,
                          sf::Vector2f{videoMode.size}});
}

// Constructors, destructor
Game::Game() {
  initWindowConfig();
  initKeybinds();
  initManagers();
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
    uiManager.handleEvent(*event);
    event->visit(
        [&](auto&& event) { eventManager.sink<decltype(event)>()(event); });
  }
}

void Game::update(sf::Time dt) {
  handleEvents();
  state->update(dt);
  audioManager.clearStoppedSounds();
  if (auto next_state = state->next_state) {
    // NOTE(des): another hack?
    uiManager.setActiveState("loading");
    render();
    uiManager.setActiveState();
    state->exit();
    audioManager.clear();
    delete state;
    state = next_state;
    state->enter();
  }
}

void Game::render() {
  window.clear();
  state->render();
  uiManager.render();
  window.display();
}

void Game::run() {
  while (window.isOpen()) {
    update(clock.restart());
    render();
  }
}
