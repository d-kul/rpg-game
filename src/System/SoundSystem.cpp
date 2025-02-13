#include "SoundSystem.h"

#include <SFML/Audio/Sound.hpp>

void SoundSystem::clearStopped(entt::registry& registry) const {
  auto view = registry.view<sf::Sound>();
  for (auto [entity, sound] : view.each()) {
    if (sound.getStatus() == sf::Sound::Status::Stopped) {
      registry.erase<sf::Sound>(entity);
      registry.destroy(entity);
    }
  }
}

void SoundSystem::clearAll(entt::registry& registry) const {
  auto view = registry.view<sf::Sound>();
  for (auto entity : view) {
    registry.destroy(entity);
  }
}
