#include "RenderSystem.h"

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "Component/DrawableComponent.h"

void RenderSystem::render(entt::registry& registry,
                          sf::RenderTarget& target) const {
  auto view = registry.view<DrawableComponent>();
  for (auto entity : view) {
    sf::RenderStates states = sf::RenderStates::Default;
    if (auto transform = registry.try_get<sf::Transformable>(entity)) {
      states.transform *= transform->getTransform();
    }
    if (auto shape = registry.try_get<sf::RectangleShape>(entity)) {
      target.draw(*shape, states);
    } else if (auto sprite = registry.try_get<sf::Sprite>(entity)) {
      target.draw(*sprite, states);
    } else if (auto text = registry.try_get<sf::Text>(entity)) {
      target.draw(*text, states);
    } else if (auto button = registry.try_get<Button>(entity)) {
      target.draw(*button, states);
    }
  }
}
