#pragma once

#include <entt/entt.hpp>

class SoundSystem {
 public:
  void clearStopped(entt::registry& registry) const;
  void clearAll(entt::registry& registry) const;
  void track(entt::registry& registry) const;
 private:
  mutable std::string tmp, tmp2;
};
