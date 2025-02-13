#include "RenderSystem.h"

void RenderSystem::render(entt::registry& registry,
                          sf::RenderTarget& target) const {
  
  auto view = registry.view<sf::Sprite, sf::Transformable>();
  for (auto [entity, sprite, transform] : view.each()) {
    target.draw(sprite, transform.getTransform());
  }
}
