#include "MainMenuState.h"

#include <SFML/Window.hpp>
#include <iostream>
#include <string>

#include "State/GameState.h"

// Constructors, destructor
MainMenuState::MainMenuState()
    : keybinds(Game::getKeybinds()), states(Game::getStates()) {
  std::cout << "MainMenuState::MainMenuState()" << '\n';
  background.setSize(sf::Vector2f{Game::getWindow().getSize()});
  background.setTexture(&background_texture);

  text.setPosition({20.f, 20.f});
  text.setFillColor(sf::Color::Black);
  std::string textString;
  textString += Game::key_storage.at(keybinds["QUIT"]);
  textString += " - exit to menu\n";
  textString += Game::key_storage.at(keybinds["MOVE_UP"]);
  textString += Game::key_storage.at(keybinds["MOVE_LEFT"]);
  textString += Game::key_storage.at(keybinds["MOVE_DOWN"]);
  textString += Game::key_storage.at(keybinds["MOVE_RIGHT"]);
  textString += " - move\n";
  textString += Game::key_storage.at(keybinds["MAKE_SOUND"]);
  textString += " - vine boom";
  text.setString(std::move(textString));

  auto screen_middle = sf::Vector2f{Game::getWindow().getSize()} * 0.5f;

  start_button.setOrigin(start_button.getGlobalBounds().getCenter());
  start_button.setPosition(screen_middle + sf::Vector2f{0.f, -80.f});

  settings_button.setOrigin(exit_button.getGlobalBounds().getCenter());
  settings_button.setPosition(screen_middle + sf::Vector2f{0.f, 0.f});

  exit_button.setOrigin(exit_button.getGlobalBounds().getCenter());
  exit_button.setPosition(screen_middle + sf::Vector2f{0.f, 80.f});

  start_button.setOnClick([&]() { states.emplace(new GameState{}); });
  exit_button.setOnClick([&]() { startQuit(); });
}

MainMenuState::~MainMenuState() {
  if (!isQuit()) {
    endState();
  }
  std::cout << "MainMenuState::~MainMenuState()" << '\n';
}

// Functionality
void MainMenuState::endState() {
  std::cout << "MainMenuState::endState()" << '\n';
}

void MainMenuState::onEvent(const sf::Event& event) {
  start_button.onEvent(event);
  settings_button.onEvent(event);
  exit_button.onEvent(event);
}

void MainMenuState::update(sf::Time dt) {}

void MainMenuState::render(sf::RenderTarget& target) {
  target.draw(background);
  target.draw(text);
  target.draw(start_button);
  target.draw(settings_button);
  target.draw(exit_button);
}
