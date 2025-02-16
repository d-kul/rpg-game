#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Button.h"
#include "State.h"

class MainMenuState : public State {
 private:
  // Lifetime management
  void loadResources();
  void loadAssets();
  void unloadResources();
  void unloadAssets();

 public:
  // Constructors, destructor
  MainMenuState(const State& other);
  MainMenuState(keybinds_t& keybinds, sf::RenderWindow& window);

  // State lifetime
  void enter() override;
  void exit() override;

  // Functionality
  void update(sf::Time dt) override;
  void render() override;

  // Listeners
  void onStart();
  void onSettings();
  void onQuit();

 private:
  // Resources
  sf::Font* font = nullptr;
  sf::Font* mono_font = nullptr;
  sf::Texture* background_texture = nullptr;

  // Members
  sf::RectangleShape background;
  sf::Text* text = nullptr;
  Button *start_button = nullptr, *settings_button = nullptr,
         *quit_button = nullptr;
};
