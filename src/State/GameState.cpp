#include "GameState.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <iostream>

#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/SoundComponent.h"
#include "Component/SoundTrackerComponent.h"
#include "Game.h"
#include "State/MainMenuState.h"

using namespace entt::literals;

// Lifetime management
void GameState::loadResources() {
  background_texture =
      Game::load<sf::Texture>("textures/test"_hs, "resources/images/test.jpg",
                              false, sf::IntRect({0, 2}, {190, 162}));
  background_texture->setRepeated(true);
  player_texture = Game::load<sf::Texture>("textures/pearto"_hs,
                                           "resources/images/pearto.png");
  font =
      Game::load<sf::Font>("fonts/papyrus"_hs, "resources/fonts/papyrus.ttf");
  mono_font = Game::load<sf::Font>("fonts/DroidSansMono"_hs,
                                   "resources/fonts/DroidSansMono.ttf");
  music = Game::load<sf::Music>("music/teto scatman"_hs,
                                "resources/music/teto scatman.ogg");
  sound_buffer = Game::load<sf::SoundBuffer>("sounds/vine_boom.wav"_hs,
                                             "resources/sounds/vine_boom.wav");
  music->setLooping(true);
}

void GameState::loadAssets() {
  auto& background = emplaceDrawable<sf::RectangleShape>();
  auto player = create();
  registry.emplace<DrawableComponent>(player);
  auto& text =
      emplaceDrawable<sf::Text>(*font, "game design is my passion", 40);
  sounds_text = &emplaceDrawable<sf::Text>(*font);

  background.setSize(sf::Vector2f{window.getSize()});
  background.setTexture(&*background_texture);

  background.setTextureRect(
      sf::IntRect{{0u, 0u}, sf::Vector2i{window.getSize()}});

  registry.emplace<sf::Sprite>(player, *player_texture);
  registry.emplace<sf::Transformable>(player);
  registry.emplace<MovementComponent>(player);
  registry.emplace<PlayerComponent>(player, sound_buffer);

  text.setFillColor(sf::Color::Green);
  text.setOutlineColor(sf::Color::Blue);
  text.setOutlineThickness(3.f);
  text.setStyle(sf::Text::Bold);
  text.setPosition({80.f, 400.f});

  sounds_text->setFillColor(sf::Color::Green);
  sounds_text->setOutlineColor(sf::Color::Blue);
  sounds_text->setOutlineThickness(1.5f);
  soundsHook(registry, entt::null);

  auto sound_tracking_entity = registry.create();
  registry.emplace<DrawableComponent>(sound_tracking_entity);
  registry.emplace<SoundTrackerComponent>(sound_tracking_entity);
  auto& sound_tracking_text =
      registry.emplace<sf::Text>(sound_tracking_entity, *mono_font, "", 14);
  sound_tracking_text.setFillColor(sf::Color::Green);
  sound_tracking_text.setOutlineColor(sf::Color::Blue);
  sound_tracking_text.setOutlineThickness(1.5f);
}

// State lifetime
void GameState::enter() {
  std::cout << "GameState::enter()" << '\n';
  registry.on_construct<SoundComponent>().connect<&GameState::soundsHook>(
      *this);
  registry.on_destroy<SoundComponent>().connect<&GameState::soundsHook>(*this);
  loadResources();
  loadAssets();
  music->play();
}

void GameState::exit() {
  std::cout << "GameState::exit()" << '\n';
  registry.on_construct<SoundComponent>().disconnect<&GameState::soundsHook>(
      *this);
  registry.on_destroy<SoundComponent>().disconnect<&GameState::soundsHook>(
      *this);
  music->stop();
}

// Functionality
void GameState::render() {}

void GameState::handleEvent(const sf::Event& event) {
  if (auto key = event.getIf<sf::Event::KeyReleased>();
      key && key->code == keybinds["QUIT"]) {
    next_state = std::make_unique<MainMenuState>();
  }
}

void GameState::soundsHook(entt::registry& registry, entt::entity entity) {
  sounds = registry.storage<SoundComponent>().size();
  sounds_text->setString("vine booms: " + std::to_string(sounds));
}
