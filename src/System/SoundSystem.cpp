#include "SoundSystem.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Text.hpp>
#include <entt/core/fwd.hpp>
#include <sstream>

#include "Component/SoundComponent.h"
#include "Component/SoundTrackerComponent.h"
#include "Game.h"

void SoundSystem::clearStopped(entt::registry& registry) const {
  auto view = registry.view<SoundComponent>();
  std::vector<entt::entity> to_delete;
  for (auto [entity, sound] : view.each()) {
    if (sound.getStatus() == sf::Sound::Status::Stopped) {
      registry.destroy(entity);
    }
  }
}

void SoundSystem::clearAll(entt::registry& registry) const {
  auto view = registry.view<SoundComponent>();
  registry.destroy(view.begin(), view.end());
}

void SoundSystem::track(entt::registry& registry) const {
  auto view = registry.view<SoundTrackerComponent, sf::Text>();
  for (auto [entity, sound_tracking_text] : view.each()) {
    std::stringstream ss;
    for (auto [sound_entity, sound] : registry.view<SoundComponent>().each()) {
      ss << std::setw(10) << static_cast<entt::id_type>(sound_entity) << " : "
         << (sound.getStatus() == sf::Sound::Status::Playing
                 ? "Playing"
                 : (sound.getStatus() == sf::Sound::Status::Paused ? " Paused"
                                                                   : "Stopped"))
         << '\n';
    }

    sound_tracking_text.setString(ss.str());
    auto bounds = sound_tracking_text.getLocalBounds();
    sound_tracking_text.setPosition(
        {-bounds.position.x + Game::getWindow().getSize().x - bounds.size.x,
         0.f});
  }
}
