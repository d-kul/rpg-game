#pragma once

#include <SFML/System/Time.hpp>
#include <entt/entt.hpp>

class MovementSystem {
 public:
  MovementSystem();
  ~MovementSystem();

  void update(sf::Time dt) const;

 private:
  entt::registry& registry;
};
