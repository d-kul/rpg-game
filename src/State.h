#pragma once

#include <SFML/Graphics.hpp>

class State;

#include "Game.h"

class State {
 public:
  // Constructors, destructor
  State(Game& game);
  virtual ~State();

  // State lifetime
  virtual void enter();
  virtual void exit();

  // Functionality
  virtual void update(sf::Time dt);
  virtual void render();

 protected:
  Game& game;

 public:
  State* next_state = nullptr;
};
