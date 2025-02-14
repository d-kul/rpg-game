#include "PlayerSystem.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Component/BulletComponent.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/SoundComponent.h"
#include "Game.h"

PlayerSystem::PlayerSystem()
    : registry(Game::getRegistry()),
      keybinds(Game::getKeybinds()),
      window(Game::getWindow()) {
  registry.on_construct<PlayerComponent>().connect<&PlayerSystem::onCreate>(
      *this);
  registry.on_update<MovementComponent>().connect<&PlayerSystem::onMove>(*this);
}

PlayerSystem::~PlayerSystem() {
  registry.on_construct<PlayerComponent>().disconnect<&PlayerSystem::onCreate>(
      *this);
  registry.on_update<MovementComponent>().disconnect<&PlayerSystem::onMove>(
      *this);
}

void PlayerSystem::update(sf::Time dt) const {
  auto view = registry.view<PlayerComponent, MovementComponent, sf::Sprite,
                            sf::Transformable, sf::Clock>();
  for (auto [entity, player, movement, sprite, transform, clock] :
       view.each()) {
    if (sf::Keyboard::isKeyPressed(keybinds["MAKE_SOUND"]) &&
        clock.getElapsedTime() > PRESS_FREQ &&
        registry.storage<SoundComponent>().size() < MAX_SOUNDS) {
      clock.restart();

      auto bullet = registry.create();
      registry.emplace<BulletComponent>(bullet);
      registry.get<sf::Transformable>(bullet).setPosition(
          transform.getPosition());
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

// Listeners
void PlayerSystem::onMove(entt::registry& registry, entt::entity entity) const {
  if (registry.all_of<PlayerComponent>(entity)) {
    sf::View view = window.getDefaultView();
    view.setCenter(registry.get<sf::Transformable>(entity).getPosition());
    window.setView(view);
  }
}

void PlayerSystem::onCreate(entt::registry& registry,
                            entt::entity entity) const {
  auto& sprite = registry.emplace_or_replace<sf::Sprite>(
      entity, Game::load<sf::Texture>(PlayerComponent::texture));
  sprite.setOrigin(sprite.getGlobalBounds().getCenter());
  registry.emplace_or_replace<MovementComponent>(entity);
  registry.emplace_or_replace<sf::Clock>(entity);
}
