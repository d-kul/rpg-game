#include "PlayerSystem.h"

#include <SFML/Audio.hpp>

#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Game.h"

PlayerSystem::PlayerSystem() : keybinds(Game::getKeybinds()) {}

void PlayerSystem::update(entt::registry& registry, sf::Time dt) const {
  auto view = registry.view<PlayerComponent, MovementComponent>();
  for (auto [entity, player, movement] : view.each()) {
    if (sf::Keyboard::isKeyPressed(keybinds["MAKE_SOUND"]) &&
        player.clock.getElapsedTime() > PRESS_FREQ &&
        registry.storage<sf::Sound>().size() < MAX_SOUNDS) {
      player.clock.restart();
      auto& added_sound =
          registry.emplace<sf::Sound>(registry.create(), player.soundBuffer);
      added_sound.setPlayingOffset(sf::seconds(0.26f));
      added_sound.play();
    }

    movement.velocity = {0, 0};
    if (sf::Keyboard::isKeyPressed(keybinds["MOVE_UP"])) {
      movement.velocity.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(keybinds["MOVE_LEFT"])) {
      movement.velocity.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(keybinds["MOVE_DOWN"])) {
      movement.velocity.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(keybinds["MOVE_RIGHT"])) {
      movement.velocity.x += 1.f;
    }
    if (movement.velocity.lengthSquared() > 0.f)
      movement.velocity = movement.velocity.normalized();
    movement.velocity *= player.movementSpeed;
  }
}
