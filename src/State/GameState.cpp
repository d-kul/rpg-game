#include "GameState.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "Entity/TestEntity.h"

// Initialization
void GameState::initAssets() {
  background_texture.setRepeated(true);

  auto& window = Game::getWindow();

  background.setSize(sf::Vector2f{window.getSize()});
  background.setTexture(&background_texture);

  background.setTextureRect(
      sf::IntRect{{0u, 0u}, sf::Vector2i{window.getSize()}});

  text.setFillColor(sf::Color::Green);
  text.setOutlineColor(sf::Color::Blue);
  text.setOutlineThickness(3.f);
  text.setStyle(sf::Text::Bold);
  text.setPosition({80.f, 400.f});

  soundsText.setFillColor(sf::Color::Green);
  soundsText.setOutlineColor(sf::Color::Blue);
  soundsText.setOutlineThickness(1.5f);

  music.setLooping(true);
  music.play();
}

// Constructors, destructor
GameState::GameState()
    : keybinds(Game::getKeybinds()), entity(new TestEntity{entity_texture}) {
  std::cout << "GameState::GameState()" << '\n';
  initAssets();
}

GameState::~GameState() {
  if (!isQuit()) {
    endState();
  }
  std::cout << "GameState::~GameState()" << '\n';
}

// Functionality
void GameState::endState() { std::cout << "GameState::endState()" << '\n'; }

void GameState::onEvent(const sf::Event& event) {
  if (auto key = event.getIf<sf::Event::KeyReleased>();
      key && key->code == keybinds["QUIT"]) {
    startQuit();
  }
}

void GameState::updateInput(sf::Time dt) {
  if (sf::Keyboard::isKeyPressed(keybinds["MAKE_SOUND"]) &&
      clock.getElapsedTime() > PRESS_FREQ && sounds.size() < MAX_SOUNDS) {
    clock.restart();
    auto& added_sound = sounds.emplace_back(sound_buffer);
    added_sound.setPlayingOffset(sf::seconds(0.26f));
    added_sound.play();
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_UP"])) {
    entity->move({0.f, -dt.asSeconds()});
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_LEFT"])) {
    entity->move({-dt.asSeconds(), 0.f});
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_DOWN"])) {
    entity->move({0.f, dt.asSeconds()});
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_RIGHT"])) {
    entity->move({dt.asSeconds(), 0.f});
  }
}

void GameState::update(sf::Time dt) {
  updateInput(dt);
  for (auto it = sounds.begin(); it != sounds.end();) {
    auto cur = it++;
    if (cur->getStatus() == sf::Sound::Status::Stopped) {
      sounds.erase(cur);
    }
  }
  entity->update(dt);
  soundsText.setString("vine booms: " + std::to_string(sounds.size()));
}

void GameState::render(sf::RenderTarget& target) {
  target.draw(background);
  target.draw(text);
  target.draw(*entity);
  target.draw(soundsText);
}
