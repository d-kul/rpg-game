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
  virtual void render();
  virtual void handleEvent(const sf::Event& event);
  std::unique_ptr<State> getNext();

 protected:
  entt::entity create();

  template <typename T, typename... Args>
  T& emplace(Args&&... args) {
    return registry.emplace<T>(entities.emplace_back(registry.create()),
                               std::forward<Args>(args)...);
  }

  // Bindings
  keybinds_t& keybinds;
  sf::RenderWindow& window;
  entt::registry& registry;

  // Members
  std::vector<entt::entity> entities;
  std::unique_ptr<State> next_state;
};
