#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class RenderSystem {
 public:
  void render(entt::registry& registry, sf::RenderTarget& target) const;
};
