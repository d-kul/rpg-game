#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <entt/entt.hpp>

#include "State.h"

class GameState : public State {
 private:
  // Lifetime management
  void loadResources();
  void loadAssets();

 public:
  GameState() = default;

  // State lifetime
  void enter() override;
  void exit() override;

  // Functionality
  void render() override;
  void handleEvent(const sf::Event& event) override;

  void soundsHook(entt::registry& registry, entt::entity entity);

 private:
  // Resources
  sf::Texture background_texture, entity_texture;
  sf::Font font;
  sf::Music music;

  // Members
  sf::Sprite* player_sprite = nullptr;
  sf::Transformable* player_transform = nullptr;
  sf::RectangleShape* background = nullptr;
  sf::Text* text = nullptr;
  sf::Text* sounds_text = nullptr;
  std::size_t sounds = 0;
};
