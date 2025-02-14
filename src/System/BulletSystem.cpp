#include "BulletSystem.h"

#include "Component/BulletComponent.h"
#include "Component/MovementComponent.h"
#include "Component/SoundComponent.h"
#include "Game.h"

BulletSystem::BulletSystem() : registry(Game::getRegistry()) {
  registry.on_construct<BulletComponent>().connect<&BulletSystem::onCreate>(
      *this);
}

BulletSystem::~BulletSystem() {
  registry.on_construct<BulletComponent>().disconnect<&BulletSystem::onCreate>(
      *this);
}

void BulletSystem::onCreate(entt::registry& registry,
                            entt::entity entity) const {
  auto& sprite = registry.emplace_or_replace<sf::Sprite>(
      entity, Game::load<sf::Texture>(BulletComponent::texture));
  sprite.setOrigin(sprite.getGlobalBounds().getCenter());
  sprite.setScale({0.4f, 0.4f});

  auto& movement = registry.emplace_or_replace<MovementComponent>(entity);
  auto mouse_direction =
      sf::Vector2f{sf::Mouse::getPosition(Game::getWindow()) -
                   sf::Vector2i{Game::getWindow().getSize() / 2u}};
  if (mouse_direction.lengthSquared() > 0) {
    mouse_direction = mouse_direction.normalized();
  }
  movement.velocity = mouse_direction * 200.f;

  auto& sound = registry.emplace_or_replace<SoundComponent>(
      entity, Game::load<sf::SoundBuffer>(BulletComponent::sound_buffer));
  // sound.setPlayingOffset(sf::seconds(0.26f));
  sound.play();
}
