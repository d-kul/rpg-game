#pragma once

#include <SFML/Graphics.hpp>

#include "Core/utility.h"
#include "EventHandler.h"

class State {
 public:
  // Constructors, destructor
  State(const State& other) = default;
  State(keybinds_t& keybinds, sf::RenderWindow& window,
        EventHandler& eventHandler);
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
  EventHandler& eventHandler;

 public:
  State* next_state = nullptr;
};
