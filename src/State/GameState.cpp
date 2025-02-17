#include "GameState.h"

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <sstream>

#include "State/MainMenuState.h"

// Lifetime management
void GameState::loadResources() {
  background_texture = resourceManager.load<sf::Texture>(
      "textures/teto_plush", "resources/images/teto_plush.png");
  background_texture->setRepeated(true);
  font = resourceManager.load<sf::Font>("fonts/papyrus",
                                        "resources/fonts/papyrus.ttf");
  mono_font = resourceManager.load<sf::Font>(
      "fonts/DroidSansMono", "resources/fonts/DroidSansMono.ttf");
  music = resourceManager.load<sf::Music>(
      "music/Teto Kasane Teto", "resources/music/Teto Kasane Teto.ogg");
  music->setLooping(true);
}

void GameState::loadAssets() {
  background.setSize(sf::Vector2f{window.getSize()});
  background.setTexture(background_texture.get());
  background.setTextureRect(
      sf::IntRect{{0u, 0u}, sf::Vector2i{window.getSize()}});

  // TODO: Player...
  // auto player = create_entity();
  // registry.emplace<PlayerComponent>(player);

  main_text =
      std::make_unique<sf::Text>(*font, "game design is my passion", 40);
  main_text->setFillColor(sf::Color::Green);
  main_text->setOutlineColor(sf::Color::Blue);
  main_text->setOutlineThickness(3.f);
  main_text->setStyle(sf::Text::Bold);
  main_text->setPosition({80.f, 400.f});

  sounds_text = std::make_unique<sf::Text>(*font);
  sounds_text->setFillColor(sf::Color::Green);
  sounds_text->setOutlineColor(sf::Color::Blue);
  sounds_text->setOutlineThickness(2.f);
  setText();

  mouse_text = std::make_unique<sf::Text>(*mono_font, "", 20);
  mouse_text->setFillColor(sf::Color::White);
  mouse_text->setOutlineColor(sf::Color::Black);
  mouse_text->setOutlineThickness(2.f);
}

// State lifetime
void GameState::enter() {
  DEBUG("");
  loadResources();
  loadAssets();
  onKeyReleased_cg = eventHandler.bind<sf::Event::KeyReleased>(
      &GameState::onKeyReleased, this);
  music->play();
}

void GameState::exit() {
  DEBUG("");
  music->stop();
  onKeyReleased_cg.disconnect();
}

// Functionality
void GameState::update(sf::Time dt) {
  auto top_left = window.mapPixelToCoords({0, 0});
  sounds_text->setPosition(top_left);
  background.setPosition(top_left);
  background.setTextureRect(
      sf::IntRect{sf::FloatRect{top_left, background.getSize()}});

  auto pos_window = sf::Mouse::getPosition(window);
  auto pos_view = window.mapPixelToCoords(pos_window);
  mouse_text->setPosition(pos_view);
  std::stringstream ss;
  ss << std::setw(7) << pos_window.x << "," << std::setw(7) << pos_window.y
     << "\n"
     << std::setw(7) << std::setprecision(5) << pos_view.x << ","
     << std::setw(7) << std::setprecision(5) << pos_view.y;
  mouse_text->setString(ss.str());
}

void GameState::render() {
  window.draw(background);
  window.draw(*main_text);
  window.draw(*sounds_text);
  window.draw(*main_text);
}

// Helpers
void GameState::setText() {
  // TODO: update text on sound events
  sounds_text->setString("things: " + std::to_string(sounds));
}

// Listeners
void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == keybinds["QUIT"]) {
    next_state = new MainMenuState{};
  }
}
