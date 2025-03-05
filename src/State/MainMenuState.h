#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Entity/Background.h"
#include "State.h"

class MainMenuState : public State {
 public:
  MainMenuState(Game& game);

  // State lifetime
  void enter() override;
  void exit() override;

  // Functionality
  void update(sf::Time dt) override;
  void render() override;

 private:
  void initUI();
  void resizeUpdateUI();

 private:
  Background background;

  UIElement* mainMiddle = nullptr;
  UIElement* settingsMiddle = nullptr;
};
