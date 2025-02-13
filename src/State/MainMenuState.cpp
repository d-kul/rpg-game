#include "MainMenuState.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>
#include <entt/entt.hpp>
#include <iostream>

#include "Button.h"
#include "Game.h"
#include "State/GameState.h"

using namespace entt::literals;

// Lifetime management
void MainMenuState::loadResources() {
  font =
      Game::retain<sf::Font>("fonts/papyrus"_hs, "resources/fonts/papyrus.ttf");
  mono_font = Game::load<sf::Font>("fonts/DroidSansMono"_hs,
                                   "resources/fonts/DroidSansMono.ttf");
  background_texture = Game::retain<sf::Texture>("textures/pearto"_hs,
                                                 "resources/images/pearto.png");
}

void MainMenuState::loadAssets() {
  sf::RectangleShape& background = emplaceDrawable<sf::RectangleShape>();
  sf::Text& text = emplaceDrawable<sf::Text>(*font, "", 40);
  Button& start_button = emplaceDrawable<Button>(sf::Vector2f{200.f, 70.f},
                                                 *mono_font, "Start game");
  Button& settings_button = emplaceDrawable<Button>(sf::Vector2f{200.f, 70.f},
                                                    *mono_font, "Settings");
  Button& quit_button =
      emplaceDrawable<Button>(sf::Vector2f{200.f, 70.f}, *mono_font, "Quit");

  background.setSize(sf::Vector2f{window.getSize()});
  background.setTexture(&*background_texture);

  text.setPosition({20.f, 20.f});
  text.setFillColor(sf::Color::Black);
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
  text.setString(std::move(text_string));

  auto screen_middle = sf::Vector2f{window.getSize()} * 0.5f;

  start_button.setOrigin(start_button.getGlobalBounds().getCenter());
  start_button.setPosition(screen_middle + sf::Vector2f{0.f, -80.f});

  settings_button.setOrigin(quit_button.getGlobalBounds().getCenter());
  settings_button.setPosition(screen_middle + sf::Vector2f{0.f, 0.f});

  quit_button.setOrigin(quit_button.getGlobalBounds().getCenter());
  quit_button.setPosition(screen_middle + sf::Vector2f{0.f, 80.f});

  start_button.setOnClick([&] { next_state = std::make_unique<GameState>(); });
  quit_button.setOnClick([&] { window.close(); });
}

// State lifetime
void MainMenuState::enter() {
  std::cout << "MainMenuState::enter()" << '\n';
  loadResources();
  loadAssets();
}

void MainMenuState::exit() { std::cout << "MainMenuState::exit()" << '\n'; }

// Functionality
void MainMenuState::render() {}

void MainMenuState::handleEvent(const sf::Event& event) {}
