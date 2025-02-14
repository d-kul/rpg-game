#pragma once

#include <entt/entt.hpp>

class SoundSystem {
 public:
  SoundSystem();
  ~SoundSystem();

  void clearStopped() const;
  void clearAll() const;

 private:
  entt::registry& registry;
};
