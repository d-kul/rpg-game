#pragma once

#include "Entity/Drawable.h"
#include "Entity/Interactible.h"

class Prop : public DrawableEntity, public InteractibleEntity {
 public:
  Prop(const sf::Texture& texture,
       std::unique_ptr<AbstractSpriteSheet> spriteSheet,
       std::unique_ptr<AbstractAction> action = {})
      : DrawableEntity(texture, std::move(spriteSheet)),
        InteractibleEntity(std::move(action)) {}
  Prop(const sf::Texture& texture,
       std::unique_ptr<AbstractSpriteSheet> spriteSheet,
       const std::vector<int>& frames, float frameRate = 1.f,
       int startFrameIndex = 0, std::unique_ptr<AbstractAction> action = {})
      : DrawableEntity(texture, std::move(spriteSheet), frames, frameRate,
                       startFrameIndex),
        InteractibleEntity(std::move(action)) {}

  Prop(const sf::Texture& texture, std::unique_ptr<AbstractAction> action = {})
      : DrawableEntity(texture), InteractibleEntity(std::move(action)) {}
  Prop(const sf::Texture& texture, const sf::IntRect& rect,
       std::unique_ptr<AbstractAction> action = {})
      : DrawableEntity(texture, rect), InteractibleEntity(std::move(action)) {}
};
