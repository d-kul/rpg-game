#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Window.hpp>
#include <entt/entt.hpp>

#include "State.h"

class MainMenuState : public State {
 private:
  // Lifetime management
  void loadResources();
  void loadAssets();

 public:
  MainMenuState() = default;

  // State lifetime
  void enter() override;
  void exit() override;

  // Functionality
  void render() override;
  void handleEvent(const sf::Event& event) override;

 private:
  // Resources
  entt::resource<sf::Font> font, mono_font;
  entt::resource<sf::Texture> background_texture;
};
