#pragma once

#include <entt/entt.hpp>
#include <tuple>

class PlayerComponent {
 public:
  float movementSpeed = 600.f;
  static constexpr std::tuple<entt::id_type, const char*> texture = {
      entt::hashed_string{"textures/pearto"}, "resources/images/pearto.png"};
};
