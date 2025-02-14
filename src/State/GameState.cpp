#include "GameState.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <sstream>

#include "Component/PlayerComponent.h"
#include "Component/SoundComponent.h"
#include "Game.h"
#include "State/MainMenuState.h"

using namespace entt::literals;

// Lifetime management
void GameState::loadResources() {
  background_texture = Game::retain<sf::Texture>(
      "textures/teto_plush"_hs, "resources/images/teto_plush.png");
  background_texture->setRepeated(true);
  font =
      Game::retain<sf::Font>("fonts/papyrus"_hs, "resources/fonts/papyrus.ttf");
  mono_font = Game::retain<sf::Font>("fonts/DroidSansMono"_hs,
                                     "resources/fonts/DroidSansMono.ttf");
  music = Game::retain<sf::Music>("music/Teto Kasane Teto"_hs,
                                  "resources/music/Teto Kasane Teto.ogg");
  music->setLooping(true);
}

void GameState::loadAssets() {
  background = &registry.emplace<sf::RectangleShape>(create_entity());
  background->setSize(sf::Vector2f{window.getSize()});
  background->setTexture(&*background_texture);
  background->setTextureRect(
      sf::IntRect{{0u, 0u}, sf::Vector2i{window.getSize()}});

  auto player = create_entity();
  registry.emplace<PlayerComponent>(player);

  auto& text = registry.emplace<sf::Text>(create_entity(), *font,
                                          "game design is my passion", 40);
  text.setFillColor(sf::Color::Green);
  text.setOutlineColor(sf::Color::Blue);
  text.setOutlineThickness(3.f);
  text.setStyle(sf::Text::Bold);
  text.setPosition({80.f, 400.f});

  sounds_text = &registry.emplace<sf::Text>(create_entity(), *font);
  sounds_text->setFillColor(sf::Color::Green);
  sounds_text->setOutlineColor(sf::Color::Blue);
  sounds_text->setOutlineThickness(2.f);
  setText();

  mouse_text = &registry.emplace<sf::Text>(create_entity(), *mono_font, "", 20);
  mouse_text->setFillColor(sf::Color::White);
  mouse_text->setOutlineColor(sf::Color::Black);
  mouse_text->setOutlineThickness(2.f);

  auto& dispatcher = Game::getDispatcher();
  dispatcher.sink<sf::Event::KeyReleased>().connect<&GameState::onKeyReleased>(
      *this);
  registry.on_construct<SoundComponent>().connect<&GameState::onSoundConstruct>(
      *this);
  registry.on_destroy<SoundComponent>().connect<&GameState::onSoundDestroy>(
      *this);
}

// State lifetime
void GameState::enter() {
  std::cout << "GameState::enter()" << '\n';
  loadResources();
  loadAssets();
  music->play();
}

void GameState::exit() {
  std::cout << "GameState::exit()" << '\n';
  auto& dispatcher = Game::getDispatcher();
  dispatcher.sink<sf::Event::KeyReleased>()
      .disconnect<&GameState::onKeyReleased>(*this);
  registry.on_construct<SoundComponent>()
      .disconnect<&GameState::onSoundConstruct>(*this);
  registry.on_destroy<SoundComponent>().disconnect<&GameState::onSoundDestroy>(
      *this);
  music->stop();
}

// Functionality
void GameState::update() {
  auto top_left = window.mapPixelToCoords({0, 0});
  sounds_text->setPosition(top_left);
  background->setPosition(top_left);
  background->setTextureRect(
      sf::IntRect{sf::FloatRect{top_left, background->getSize()}});

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

// Helpers
void GameState::setText() {
  sounds_text->setString("things: " + std::to_string(sounds));
}

// Listeners
void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == keybinds["QUIT"]) {
    next_state = std::make_unique<MainMenuState>();
  }
}

void GameState::onSoundConstruct(entt::registry& registry,
                                 entt::entity entity) {
  ++sounds;
  setText();
}

void GameState::onSoundDestroy(entt::registry& registry, entt::entity entity) {
  --sounds;
  setText();
}
