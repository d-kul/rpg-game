#pragma once

#include <entt/entt.hpp>
#include <tuple>

class BulletComponent {
 public:
  static constexpr std::tuple<entt::id_type, const char*> texture = {
      entt::hashed_string{"textures/pearto"}, "resources/images/pearto.png"};
  static constexpr std::tuple<entt::id_type, const char*> sound_buffer = {
      entt::hashed_string{"sounds/huh"}, "resources/sounds/huh.wav"};
};
