#include "EventHandlerSystem.h"

#include <SFML/Audio/Sound.hpp>

#include "Button.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Game.h"

void EventHandlerSystem::handleEvent(entt::registry& registry,
                                     sf::Event event) const {
  auto ui_view = registry.view<Button>();
  for (auto [entity, button] : ui_view.each()) {
    button.handleEvent(event);
  }
}
