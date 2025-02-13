#pragma once

#include <SFML/System/Time.hpp>
#include <entt/entt.hpp>

class MovementSystem {
 public:
  void update(entt::registry& registry, sf::Time dt) const;
};
