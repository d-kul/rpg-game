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
  void update() override;

  // Helpers
  void setText();

  // Listeners
  void onKeyReleased(sf::Event::KeyReleased keyReleased);
  void onSoundConstruct(entt::registry& registry, entt::entity entity);
  void onSoundDestroy(entt::registry& registry, entt::entity entity);

 private:
  // Resources
  entt::resource<sf::Texture> background_texture;
  entt::resource<sf::Font> font, mono_font;
  entt::resource<sf::Music> music;

  // Members
  sf::RectangleShape* background = nullptr;
  sf::Text* sounds_text = nullptr;
  sf::Text* mouse_text = nullptr;
  std::size_t sounds = 0;
};
