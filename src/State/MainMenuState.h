#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Window.hpp>
#include <entt/entt.hpp>
#include <memory>

#include "Button.h"
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
  sf::Font font;
  sf::Font mono_font;
  sf::Texture background_texture;

  // Members
  sf::RectangleShape* background = nullptr;
  sf::Text* text = nullptr;
  Button *start_button = nullptr, *settings_button = nullptr,
         *quit_button = nullptr;
};
