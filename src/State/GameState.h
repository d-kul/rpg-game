#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

#include "Level.h"
#include "State.h"

class GameState : public State {
 private:
  // Lifetime management
  void loadResources();
  void loadLevel();

 public:
  GameState(Game& game);

  // State lifetime
  void enter() override;
  void exit() override;

  // Functionality
  void update(sf::Time dt) override;
  void render() override;

 private:
  void initUI();

  void initImageFrame();
  void initTextFrame();
  void initMenuFrame();

  friend class LevelAction;

 private:
  // Resources
  std::shared_ptr<sf::Font> dialogFont;
  std::shared_ptr<sf::Font> menuFont;

  Level level;

  UIElement* mainFrame;
};
