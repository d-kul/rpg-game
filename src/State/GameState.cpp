#include "GameState.h"

#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <iostream>

#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "State/MainMenuState.h"

// Lifetime management
void GameState::loadResources() {
  bool success = true;
  success &= background_texture.loadFromFile("resources/images/test.jpg", false,
                                             sf::IntRect({0, 2}, {190, 162}));
  background_texture.setRepeated(true);
  success &= entity_texture.loadFromFile("resources/images/pearto.png");
  success &= font.openFromFile("resources/fonts/papyrus.ttf");
  success &= music.openFromFile("resources/music/teto scatman.ogg");

  music.setLooping(true);
}

void GameState::loadAssets() {
  background = &emplace<sf::RectangleShape>();
  text = &emplace<sf::Text>(font, "game design is my passion", 40);
  sounds_text = &emplace<sf::Text>(font);

  background->setSize(sf::Vector2f{window.getSize()});
  background->setTexture(&background_texture);

  background->setTextureRect(
      sf::IntRect{{0u, 0u}, sf::Vector2i{window.getSize()}});

  text->setFillColor(sf::Color::Green);
  text->setOutlineColor(sf::Color::Blue);
  text->setOutlineThickness(3.f);
  text->setStyle(sf::Text::Bold);
  text->setPosition({80.f, 400.f});

  sounds_text->setFillColor(sf::Color::Green);
  sounds_text->setOutlineColor(sf::Color::Blue);
  sounds_text->setOutlineThickness(1.5f);
  soundsHook(registry, entt::null);

  auto player_entity = create();
  player_transform = &registry.emplace<sf::Transformable>(player_entity);
  player_sprite = &registry.emplace<sf::Sprite>(player_entity, entity_texture);
  registry.emplace<MovementComponent>(player_entity);
  registry.emplace<PlayerComponent>(
      player_entity, sf::SoundBuffer{"resources/sounds/vine_boom.wav"});

  music.play();
}

// State lifetime
void GameState::enter() {
  std::cout << "GameState::enter()" << '\n';
  registry.on_construct<sf::Sound>().connect<&GameState::soundsHook>(*this);
  registry.on_destroy<sf::Sound>().connect<&GameState::soundsHook>(*this);
  loadResources();
  loadAssets();
}

void GameState::exit() {
  std::cout << "GameState::exit()" << '\n';
  registry.on_construct<sf::Sound>().disconnect<&GameState::soundsHook>(*this);
  registry.on_destroy<sf::Sound>().disconnect<&GameState::soundsHook>(*this);
}

// Functionality
void GameState::render() {
  window.draw(*background);
  window.draw(*player_sprite, player_transform->getTransform());
  window.draw(*text);
  sounds_text->setString("vine booms: " + std::to_string(sounds));
  window.draw(*sounds_text);
}

void GameState::handleEvent(const sf::Event& event) {
  if (auto key = event.getIf<sf::Event::KeyReleased>();
      key && key->code == keybinds["QUIT"]) {
    next_state = std::make_unique<MainMenuState>();
  }
}

void GameState::soundsHook(entt::registry& registry, entt::entity entity) {
  sounds = registry.storage<sf::Sound>().size();
}
