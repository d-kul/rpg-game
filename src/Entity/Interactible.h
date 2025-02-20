#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

class Interactible;

#include "Entity.h"
#include "Manager/Interactible.h"

class Interactible : public Entity {
 public:
  static constexpr float tileSize = 64.f;
  using action_t = std::function<void()>;

 public:
  Interactible(sf::Vector2i position);

  void setPosition(sf::Vector2i position);
  void setPosition(sf::Vector2f position) = delete;

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  action_t action;
  std::unique_ptr<sf::Drawable> drawable;

 private:
  InteractibleManager& manager;
};
