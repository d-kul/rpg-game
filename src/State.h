#pragma once

#include <SFML/Graphics.hpp>

#include "utility.h"

class State {
 public:
  // Constructors, destructor
  State(const State& other);
  State(keybinds_t& keybinds, sf::RenderWindow& window);
  virtual ~State();

  // State lifetime
  virtual void enter();
  virtual void exit();

  // Functionality
  virtual void update(sf::Time dt);
  virtual void render();

 protected:
  // Bindings
  keybinds_t& keybinds;
  sf::RenderWindow& window;

 public:
  State* next_state;
};
