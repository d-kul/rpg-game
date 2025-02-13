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
  entt::resource<sf::Texture> background_texture, player_texture;
  entt::resource<sf::Font> font, mono_font;
  entt::resource<sf::Music> music;
  entt::resource<sf::SoundBuffer> sound_buffer;

  // Members
  sf::Text* sounds_text = nullptr;
  std::size_t sounds = 0;
};
