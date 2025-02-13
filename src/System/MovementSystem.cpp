#include "MovementSystem.h"

#include <SFML/Graphics/Transformable.hpp>

#include "Component/MovementComponent.h"

void MovementSystem::update(entt::registry& registry, sf::Time dt) const {
  auto view = registry.view<MovementComponent, sf::Transformable>();
  for (auto [entity, movement, transform] : view.each()) {
    transform.move(movement.velocity * dt.asSeconds());
    movement.velocity += movement.acceleration * dt.asSeconds();
  }
}
