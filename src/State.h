#pragma once

#include <SFML/Graphics.hpp>

#include "Core/utility.h"
#include "EventHandler.h"
#include "ResourceManager.h"

class State {
 public:
  // Constructors, destructor
  State();
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
  ResourceManager& resourceManager;

 public:
  State* next_state = nullptr;
};
