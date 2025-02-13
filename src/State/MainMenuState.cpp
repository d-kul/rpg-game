#include "MainMenuState.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "Button.h"
#include "Game.h"
#include "State/GameState.h"

// Lifetime management
void MainMenuState::loadResources() {
  bool success = true;
  success &= font.openFromFile("resources/fonts/papyrus.ttf");
  success &= mono_font.openFromFile("resources/fonts/DroidSansMono.ttf");
  success &= background_texture.loadFromFile("resources/images/pearto.png");
  // handle errors?
}

void MainMenuState::loadAssets() {
  background = &emplace<sf::RectangleShape>();
  text = &emplace<sf::Text>(font, "", 40);
  start_button =
      &emplace<Button>(sf::Vector2f{200.f, 70.f}, mono_font, "Start game");
  settings_button =
      &emplace<Button>(sf::Vector2f{200.f, 70.f}, mono_font, "Settings");
  quit_button = &emplace<Button>(sf::Vector2f{200.f, 70.f}, mono_font, "Quit");

  background->setSize(sf::Vector2f{window.getSize()});
  background->setTexture(&background_texture);

  text->setPosition({20.f, 20.f});
  text->setFillColor(sf::Color::Black);
  std::string text_string;
  text_string += Game::key_storage.at(keybinds["QUIT"]);
  text_string += " - exit to menu\n";
  text_string += Game::key_storage.at(keybinds["MOVE_UP"]);
  text_string += Game::key_storage.at(keybinds["MOVE_LEFT"]);
  text_string += Game::key_storage.at(keybinds["MOVE_DOWN"]);
  text_string += Game::key_storage.at(keybinds["MOVE_RIGHT"]);
  text_string += " - move\n";
  text_string += Game::key_storage.at(keybinds["MAKE_SOUND"]);
  text_string += " - vine boom";
  text->setString(std::move(text_string));

  auto screen_middle = sf::Vector2f{window.getSize()} * 0.5f;

  start_button->setOrigin(start_button->getGlobalBounds().getCenter());
  start_button->setPosition(screen_middle + sf::Vector2f{0.f, -80.f});

  settings_button->setOrigin(quit_button->getGlobalBounds().getCenter());
  settings_button->setPosition(screen_middle + sf::Vector2f{0.f, 0.f});

  quit_button->setOrigin(quit_button->getGlobalBounds().getCenter());
  quit_button->setPosition(screen_middle + sf::Vector2f{0.f, 80.f});

  start_button->setOnClick([&] { next_state = std::make_unique<GameState>(); });
  quit_button->setOnClick([&] { window.close(); });
}

// State lifetime
void MainMenuState::enter() {
  std::cout << "MainMenuState::enter()" << '\n';
  loadResources();
  loadAssets();
}

void MainMenuState::exit() { std::cout << "MainMenuState::exit()" << '\n'; }

// Functionality
void MainMenuState::render() {
  window.draw(*background);
  window.draw(*text);
  window.draw(*start_button);
  window.draw(*settings_button);
  window.draw(*quit_button);
}

void MainMenuState::handleEvent(const sf::Event& event) {}
