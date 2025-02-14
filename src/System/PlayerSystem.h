#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

#include "utility.h"

class PlayerSystem {
 public:
  PlayerSystem();
  ~PlayerSystem();

  void update(sf::Time dt) const;

  // Listeners
  void onMove(entt::registry& registry, entt::entity entity) const;
  void onCreate(entt::registry& registry, entt::entity entity) const;

 private:
  entt::registry& registry;
  keybinds_t& keybinds;
  sf::RenderWindow& window;

  static constexpr sf::Time PRESS_FREQ = sf::milliseconds(70);
  static constexpr size_t MAX_SOUNDS = 2000u;
};
