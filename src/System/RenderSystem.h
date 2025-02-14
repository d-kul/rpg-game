#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class RenderSystem {
 public:
  RenderSystem();
  ~RenderSystem();

  void subscribe() const;
  void unsubscribe() const;

  void render(sf::RenderTarget& target) const;

  // Listeners
  void sortElements() const;

 private:
  entt::registry& registry;
};
