#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

#include "Entity/Background.h"
#include "UI/Button.h"
#include "State.h"

class MainMenuState : public State {
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

  // Listeners
  void onStart();
  void onSettings();
  void onQuit();

 private:
  // Resources
  std::shared_ptr<sf::Font> font;
  std::shared_ptr<sf::Font> mono_font;
  std::shared_ptr<sf::Texture> background_texture;

  // Members
  Background background;
  std::unique_ptr<sf::Text> text;
  std::unique_ptr<Button> start_button, settings_button, quit_button;
};
