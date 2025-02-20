#include "MainMenuState.h"

#include "Core/Logger.h"
#include "Game.h"
#include "State/GameState.h"

// Lifetime management
void MainMenuState::loadResources() {
  font = resourceManager.retain<sf::Font>("fonts/papyrus",
                                          "resources/fonts/papyrus.ttf");
  mono_font = resourceManager.retain<sf::Font>(
      "fonts/DroidSansMono", "resources/fonts/DroidSansMono.ttf");
  background_texture = resourceManager.retain<sf::Texture>(
      "textures/pearto", "resources/images/pearto.png");
}

void MainMenuState::loadAssets() {
  background.setSize(sf::Vector2f{windowSize});
  background.setTexture(background_texture.get(), sf::Vector2f{windowSize});

  text = std::make_unique<sf::Text>(*font, "", 40);
  text->setPosition({20.f, 20.f});
  text->setFillColor(sf::Color::Black);
  auto& keys = Game::key_storage;
  std::stringstream ss;
  ss << keys.at(keybinds["QUIT"]) << " - exit to menu\n"
     << keys.at(keybinds["MOVE_UP"]) << keys.at(keybinds["MOVE_LEFT"])
     << keys.at(keybinds["MOVE_DOWN"]) << keys.at(keybinds["MOVE_RIGHT"])
     << " - move\n"
     << keys.at(keybinds["INTERACT"]) << " - interact";
  text->setString(ss.str());

  auto screen_middle = sf::Vector2f{windowSize} * 0.5f;

  start_button = std::make_unique<Button>(*mono_font, sf::Vector2f{200.f, 70.f},
                                          "Start game");
  start_button->setOrigin(start_button->getGlobalBounds().getCenter());
  start_button->setPosition(screen_middle + sf::Vector2f{0.f, -80.f});
  start_button->onClick().bind(&MainMenuState::onStart, this);

  settings_button = std::make_unique<Button>(
      *mono_font, sf::Vector2f{200.f, 70.f}, "Settings");
  settings_button->setOrigin(settings_button->getGlobalBounds().getCenter());
  settings_button->setPosition(screen_middle + sf::Vector2f{0.f, 0.f});
  settings_button->onClick().bind(&MainMenuState::onSettings, this);

  quit_button =
      std::make_unique<Button>(*mono_font, sf::Vector2f{200.f, 70.f}, "Quit");
  quit_button->setOrigin(quit_button->getGlobalBounds().getCenter());
  quit_button->setPosition(screen_middle + sf::Vector2f{0.f, 80.f});
  quit_button->onClick().bind(&MainMenuState::onQuit, this);
}

// State lifetime
void MainMenuState::enter() {
  DEBUG("entering MainMenuState");
  loadResources();
  loadAssets();
  window.setView(
      sf::View{sf::Vector2f{windowSize} / 2.f, sf::Vector2f{windowSize}});
  background.setPosition(window.getView().getCenter());
}

void MainMenuState::exit() { DEBUG("entering MainMenuState"); }

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
void MainMenuState::onStart() { next_state = new GameState{}; }
void MainMenuState::onSettings() {}
void MainMenuState::onQuit() { window.close(); }
