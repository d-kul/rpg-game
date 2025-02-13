#pragma once

#include <SFML/System/Time.hpp>
#include <entt/entt.hpp>

#include "utility.h"

class PlayerSystem {
 public:
  PlayerSystem();
  void update(entt::registry& registry, sf::Time dt) const;

 private:
  keybinds_t& keybinds;

  static constexpr sf::Time PRESS_FREQ = sf::milliseconds(70);
  static constexpr size_t MAX_SOUNDS = 2000u;
};
