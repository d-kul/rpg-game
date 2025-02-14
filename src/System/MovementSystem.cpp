#include "MovementSystem.h"

#include <SFML/Graphics/Transformable.hpp>
#include <entt/entity/fwd.hpp>

#include "Component/MovementComponent.h"
#include "Game.h"

MovementSystem::MovementSystem() : registry(Game::getRegistry()) {
  registry.on_construct<MovementComponent>()
      .connect<&entt::registry::emplace_or_replace<sf::Transformable>>();
}

MovementSystem::~MovementSystem() {
  registry.on_construct<MovementComponent>()
      .disconnect<&entt::registry::emplace_or_replace<sf::Transformable>>();
}

void MovementSystem::update(sf::Time dt) const {
  auto view = registry.view<MovementComponent, sf::Transformable>();
  for (auto [entity, movement, transform] : view.each()) {
    transform.move(movement.velocity * dt.asSeconds());
    movement.velocity += movement.acceleration * dt.asSeconds();
    registry.patch<MovementComponent>(entity);
  }
}
