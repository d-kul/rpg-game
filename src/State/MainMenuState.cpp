#include "MainMenuState.h"

#include <iostream>

#include "Button.h"
#include "Game.h"
#include "State/GameState.h"

// Lifetime management
void MainMenuState::loadResources() {
  font = new sf::Font("resources/fonts/papyrus.ttf");
  mono_font = new sf::Font("resources/fonts/DroidSansMono.ttf");
  background_texture = new sf::Texture("resources/images/pearto.png");
}

void MainMenuState::loadAssets() {
  background.setSize(sf::Vector2f{window.getSize()});
  background.setTexture(&*background_texture);

  text = new sf::Text(*font, "", 40);
  text->setPosition({20.f, 20.f});
  text->setFillColor(sf::Color::Black);
  auto& keys = Game::key_storage;
  std::stringstream ss;
  ss << keys.at(keybinds["QUIT"]) << " - exit to menu\n"
     << keys.at(keybinds["MOVE_UP"]) << keys.at(keybinds["MOVE_LEFT"])
     << keys.at(keybinds["MOVE_DOWN"]) << keys.at(keybinds["MOVE_RIGHT"])
     << " - move\n"
     << keys.at(keybinds["MAKE_SOUND"]) << " - shoot a thing";
  text->setString(ss.str());

  auto screen_middle = sf::Vector2f{window.getSize()} * 0.5f;

  start_button =
      new Button(window, *mono_font, sf::Vector2f{200.f, 70.f}, "Start game");
  start_button->setOrigin(start_button->getGlobalBounds().getCenter());
  start_button->setPosition(screen_middle + sf::Vector2f{0.f, -80.f});
  start_button->onClick().emplace_back(
      std::bind(&MainMenuState::onStart, this));

  settings_button =
      new Button(window, *mono_font, sf::Vector2f{200.f, 70.f}, "Settings");
  settings_button->setOrigin(settings_button->getGlobalBounds().getCenter());
  settings_button->setPosition(screen_middle + sf::Vector2f{0.f, 0.f});
  settings_button->onClick().emplace_back(
      std::bind(&MainMenuState::onSettings, this));

  quit_button =
      new Button(window, *mono_font, sf::Vector2f{200.f, 70.f}, "Quit");
  quit_button->setOrigin(quit_button->getGlobalBounds().getCenter());
  quit_button->setPosition(screen_middle + sf::Vector2f{0.f, 80.f});
  quit_button->onClick().emplace_back(std::bind(&MainMenuState::onQuit, this));
}

void MainMenuState::unloadResources() {
  delete font;
  delete mono_font;
  delete background_texture;
}

void MainMenuState::unloadAssets() {
  delete text;
  delete start_button;
  delete settings_button;
  delete quit_button;
}

// Constructors, destructor
MainMenuState::MainMenuState(const State& other) : State(other) {}

MainMenuState::MainMenuState(keybinds_t& keybinds, sf::RenderWindow& window)
    : State(keybinds, window) {}

// State lifetime
void MainMenuState::enter() {
  std::cout << "MainMenuState::enter()" << '\n';
  loadResources();
  loadAssets();
  window.setView(window.getDefaultView());
}

void MainMenuState::exit() {
  std::cout << "MainMenuState::exit()" << '\n';
  unloadAssets();
  unloadResources();
}

// Functionality
void MainMenuState::update(sf::Time dt) {}

void MainMenuState::render() {
  window.draw(background);
  window.draw(*text);
  window.draw(*start_button);
  window.draw(*settings_button);
  window.draw(*quit_button);
}

// Listeners
void MainMenuState::onStart() { next_state = new GameState(*this); }

void MainMenuState::onSettings() {}

void MainMenuState::onQuit() { window.close(); }
