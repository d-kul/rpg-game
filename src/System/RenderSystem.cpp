#include "RenderSystem.h"

#include "Button.h"
#include "Component/DrawableComponent.h"
#include "Game.h"

RenderSystem::RenderSystem() : registry(Game::getRegistry()) {
  registry.on_construct<sf::RectangleShape>()
      .connect<&entt::registry::emplace_or_replace<DrawableComponent>>();
  registry.on_construct<sf::Sprite>()
      .connect<&entt::registry::emplace_or_replace<DrawableComponent>>();
  registry.on_construct<sf::Text>()
      .connect<&entt::registry::emplace_or_replace<DrawableComponent>>();
  registry.on_construct<Button>()
      .connect<&entt::registry::emplace_or_replace<DrawableComponent>>();
}

RenderSystem::~RenderSystem() {
  registry.on_construct<sf::RectangleShape>()
      .disconnect<&entt::registry::emplace_or_replace<DrawableComponent>>();
  registry.on_construct<sf::Sprite>()
      .disconnect<&entt::registry::emplace_or_replace<DrawableComponent>>();
  registry.on_construct<sf::Text>()
      .disconnect<&entt::registry::emplace_or_replace<DrawableComponent>>();
  registry.on_construct<Button>()
      .disconnect<&entt::registry::emplace_or_replace<DrawableComponent>>();
}

void RenderSystem::subscribe() const {
  registry.on_construct<DrawableComponent>()
      .connect<&RenderSystem::sortElements>(*this);
  registry.on_destroy<entt::entity>().connect<&RenderSystem::sortElements>(
      *this);
}

void RenderSystem::unsubscribe() const {
  registry.on_construct<DrawableComponent>()
      .disconnect<&RenderSystem::sortElements>(*this);
  registry.on_destroy<entt::entity>().disconnect<&RenderSystem::sortElements>(
      *this);
}

void RenderSystem::render(sf::RenderTarget& target) const {
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

void RenderSystem::sortElements() const {
  registry.sort<DrawableComponent>(std::less<>{});
}
