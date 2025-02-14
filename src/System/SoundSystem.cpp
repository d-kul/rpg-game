#include "SoundSystem.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Text.hpp>
#include <entt/core/fwd.hpp>

#include "Component/SoundComponent.h"
#include "Game.h"

SoundSystem::SoundSystem() : registry(Game::getRegistry()) {}

SoundSystem::~SoundSystem() { clearAll(); }

void SoundSystem::clearStopped() const {
  auto view = registry.view<SoundComponent>();
  for (auto [entity, sound] : view.each()) {
    if (sound.getStatus() == sf::Sound::Status::Stopped) {
      registry.destroy(entity);
    }
  }
}

void SoundSystem::clearAll() const {
  auto view = registry.view<SoundComponent>();
  registry.destroy(view.begin(), view.end());
}
