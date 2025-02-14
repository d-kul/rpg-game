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
  mono_font = Game::retain<sf::Font>("fonts/DroidSansMono"_hs,
                                     "resources/fonts/DroidSansMono.ttf");
  background_texture = Game::retain<sf::Texture>("textures/pearto"_hs,
                                                 "resources/images/pearto.png");
}

void MainMenuState::loadAssets() {
  sf::RectangleShape& background =
      registry.emplace<sf::RectangleShape>(create_entity());
  background.setSize(sf::Vector2f{window.getSize()});
  background.setTexture(&*background_texture);

  sf::Text& text = registry.emplace<sf::Text>(create_entity(), *font, "", 40);
  text.setPosition({20.f, 20.f});
  text.setFillColor(sf::Color::Black);
  auto& keys = Game::key_storage;
  std::stringstream ss;
  ss << keys.at(keybinds["QUIT"]) << " - exit to menu\n"
     << keys.at(keybinds["MOVE_UP"]) << keys.at(keybinds["MOVE_LEFT"])
     << keys.at(keybinds["MOVE_DOWN"]) << keys.at(keybinds["MOVE_RIGHT"])
     << " - move\n"
     << keys.at(keybinds["MAKE_SOUND"]) << " - shoot a thing";
  text.setString(ss.str());

  auto screen_middle = sf::Vector2f{window.getSize()} * 0.5f;

  Button& start_button = registry.emplace<Button>(
      create_entity(), sf::Vector2f{200.f, 70.f}, *mono_font, "Start game");
  start_button.setOrigin(start_button.getGlobalBounds().getCenter());
  start_button.setPosition(screen_middle + sf::Vector2f{0.f, -80.f});
  start_button.onClick().connect<&MainMenuState::onStart>(*this);

  Button& settings_button = registry.emplace<Button>(
      create_entity(), sf::Vector2f{200.f, 70.f}, *mono_font, "Settings");
  settings_button.setOrigin(settings_button.getGlobalBounds().getCenter());
  settings_button.setPosition(screen_middle + sf::Vector2f{0.f, 0.f});
  settings_button.onClick().connect<&MainMenuState::onSettings>(*this);

  Button& quit_button = registry.emplace<Button>(
      create_entity(), sf::Vector2f{200.f, 70.f}, *mono_font, "Quit");
  quit_button.setOrigin(quit_button.getGlobalBounds().getCenter());
  quit_button.setPosition(screen_middle + sf::Vector2f{0.f, 80.f});
  quit_button.onClick().connect<&MainMenuState::onQuit>(*this);
}

// State lifetime
void MainMenuState::enter() {
  std::cout << "MainMenuState::enter()" << '\n';
  loadResources();
  loadAssets();
  window.setView(window.getDefaultView());
}

void MainMenuState::exit() { std::cout << "MainMenuState::exit()" << '\n'; }

// Functionality
void MainMenuState::update() {}

// Listeners
void MainMenuState::onStart() { next_state = std::make_unique<GameState>(); }
void MainMenuState::onSettings() {}
void MainMenuState::onQuit() { window.close(); }
