#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <memory>

#include "utility.h"

class State {
 public:
  State();
  virtual ~State();

  // State lifetime
  virtual void enter();
  virtual void exit();

  // Functionality
  virtual void update();

 protected:
  entt::entity create_entity();

  // Bindings
  keybinds_t& keybinds;
  sf::RenderWindow& window;
  entt::registry& registry;

  // Members
  std::vector<entt::entity> entities;

 public:
  std::unique_ptr<State> next_state;
};
