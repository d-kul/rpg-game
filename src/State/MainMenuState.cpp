#include "MainMenuState.h"

#include <iostream>

#include "State/GameState.h"

void MainMenuState::initKeybinds() {
  keybinds.insert({"QUIT", sf::Keyboard::Key::Escape});
  keybinds.insert({"START_GAME", sf::Keyboard::Key::Enter});
}

MainMenuState::MainMenuState(std::map<std::string, sf::Keyboard::Key>& keybinds,
                             sf::Vector2f window_size,
                             std::stack<std::unique_ptr<State>>& states)
    : State(keybinds), states(states) {
  std::cout << "MainMenuState::MainMenuState()" << '\n';
  initKeybinds();
  background.setSize(window_size);
  background.setTexture(&background_texture);
  text.setPosition({20.f, 20.f});
  text.setFillColor(sf::Color::Black);
}

MainMenuState::~MainMenuState() {
  if (!isQuit()) {
    endState();
  }
  std::cout << "MainMenuState::~MainMenuState()" << '\n';
}

void MainMenuState::endState() {
  std::cout << "MainMenuState::endState()" << '\n';
}

void MainMenuState::onEvent(const sf::Event& event) {
  if (auto key = event.getIf<sf::Event::KeyReleased>();
      key && key->code == keybinds["QUIT"]) {
    startQuit();
  }
  if (auto key = event.getIf<sf::Event::KeyReleased>();
      key && key->code == keybinds["START_GAME"]) {
    states.emplace(new GameState{keybinds});
  }
}

void MainMenuState::update(sf::Time dt) {}

void MainMenuState::render(sf::RenderTarget& target) {
  target.draw(background);
  target.draw(text);
}
