#pragma once

#include "Entity/Drawable.h"
#include "Entity/Interactible.h"

class Prop : public DrawableEntity, public InteractibleEntity {
 public:
  Prop(const sf::Texture& texture, InteractibleManager& manager,
       std::unique_ptr<AbstractSpriteSheet> spriteSheet,
       Action* action = nullptr)
      : DrawableEntity(texture, std::move(spriteSheet)),
        InteractibleEntity(manager, action) {}
  Prop(const sf::Texture& texture, InteractibleManager& manager,
       std::unique_ptr<AbstractSpriteSheet> spriteSheet,
       const std::vector<int>& frames, float frameRate = 1.f,
       int startFrameIndex = 0, Action* action = nullptr)
      : DrawableEntity(texture, std::move(spriteSheet), frames, frameRate,
                       startFrameIndex),
        InteractibleEntity(manager, action) {}

  Prop(const sf::Texture& texture, InteractibleManager& manager,
       Action* action = nullptr)
      : DrawableEntity(texture), InteractibleEntity(manager, action) {}
  Prop(const sf::Texture& texture, InteractibleManager& manager,
       const sf::IntRect& rect, Action* action = nullptr)
      : DrawableEntity(texture, rect), InteractibleEntity(manager, action) {}
};
