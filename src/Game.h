#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
#include <memory>
#include <stack>
#include <string>

#include "State.h"

class Game {
 private:
  // Initialization
  void initWindow();
  void initKeybinds();
  void initStates();

 public:
  // Constructors, destructor
  Game();
  virtual ~Game();

  // Functionality
  void pollEvents();
  void update(sf::Time dt);
  void render();
  void run();

 private:
  // Members
  sf::RenderWindow window;
  sf::Clock clock;

  std::stack<std::unique_ptr<State>> states;

 protected:
  std::map<std::string, sf::Keyboard::Key> keybinds;
  static const std::map<std::string, sf::Keyboard::Key> keys;
};
