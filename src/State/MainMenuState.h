#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Button.h"
#include "Game.h"
#include "State.h"

class MainMenuState : public State {
 private:
 public:
  // Constructors, destructor
  MainMenuState();
  virtual ~MainMenuState();

  // Functionality
  void endState() override;
  void onEvent(const sf::Event& event) override;
  void update(sf::Time dt) override;
  void render(sf::RenderTarget& target) override;

 private:
  // Bindings
  Game::states_t& states;
  Game::keybinds_t& keybinds;

  // Members
  sf::Texture background_texture{"resources/pearto.png"};
  sf::RectangleShape background;
  const sf::Font font{"resources/papyrus.ttf"};
  const sf::Font mono_font{"resources/DroidSansMono.ttf"};
  sf::Text text{font, "", 40};
  Button start_button{{200.f, 70.f}, mono_font, "Start game"};
  Button settings_button{{200.f, 70.f}, mono_font, "Settings"};
  Button exit_button{{200.f, 70.f}, mono_font, "Quit"};
};
