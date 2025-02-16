#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "State.h"

class GameState : public State {
 private:
  // Lifetime management
  void loadResources();
  void loadAssets();
  void unloadResources();
  void unloadAssets();

 public:
  // Constructors, destructor
  GameState(const State& other);
  GameState(keybinds_t& keybinds, sf::RenderWindow& window);

  // State lifetime
  void enter() override;
  void exit() override;

  // Functionality
  void update(sf::Time dt) override;
  void render() override;

  // Helpers
  void setText();

  // Listeners
  void onKeyReleased(sf::Event::KeyReleased keyReleased);

 private:
  // Resources
  sf::Texture* background_texture = nullptr;
  sf::Font* font = nullptr;
  sf::Font* mono_font = nullptr;
  sf::Music* music = nullptr;

  // Members
  sf::RectangleShape background;
  sf::Text* main_text = nullptr;
  sf::Text* sounds_text = nullptr;
  sf::Text* mouse_text = nullptr;
  std::size_t sounds = 0;
};
