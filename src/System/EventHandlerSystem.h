#pragma once

#include <SFML/Window/Event.hpp>
#include <entt/entt.hpp>

class EventHandlerSystem {
 public:
  void handleEvent(entt::registry& registry, sf::Event event) const;
};
