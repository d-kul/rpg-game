#pragma once

#include "Entity/Drawable.h"
#include "Entity/Interactible.h"

class Prop : public DrawableEntity, public InteractibleEntity {
 public:
  Prop(const AbstractSpriteSheet& spriteSheet, InteractibleManager& manager,
       Action* action = nullptr)
      : DrawableEntity(spriteSheet), InteractibleEntity(manager, action) {}
  Prop(const AbstractSpriteSheet& spriteSheet, InteractibleManager& manager,
       const std::vector<int>& frames, float frameRate = 1.f,
       int startFrameIndex = 0, Action* action = nullptr)
      : DrawableEntity(spriteSheet, frames, frameRate, startFrameIndex),
        InteractibleEntity(manager, action) {}

  Prop(const sf::Texture& texture, InteractibleManager& manager,
       Action* action = nullptr)
      : DrawableEntity(texture), InteractibleEntity(manager, action) {}
  Prop(const sf::Texture& texture, InteractibleManager& manager,
       const sf::IntRect& rect, Action* action = nullptr)
      : DrawableEntity(texture, rect), InteractibleEntity(manager, action) {}
};
