#pragma once

#include <entt/entt.hpp>

class BulletSystem {
 public:
  BulletSystem();
  ~BulletSystem();

  // Listeners
  void onCreate(entt::registry& registry, entt::entity entity) const;

 private:
  entt::registry& registry;
};
