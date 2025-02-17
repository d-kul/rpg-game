#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "State.h"

class GameState : public State {
 private:
  // Lifetime management
  void loadResources();
  void loadAssets();

 public:
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
  std::shared_ptr<sf::Texture> background_texture;
  std::shared_ptr<sf::Font> font;
  std::shared_ptr<sf::Font> mono_font;
  std::shared_ptr<sf::Music> music;

  // Members
  sf::RectangleShape background;
  std::unique_ptr<sf::Text> main_text;
  std::unique_ptr<sf::Text> sounds_text;
  std::unique_ptr<sf::Text> mouse_text;
  std::size_t sounds = 0;

  ConnectionGuard onKeyReleased_cg;
};
