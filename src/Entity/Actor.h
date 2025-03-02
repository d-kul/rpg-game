#pragma once

#include "Entity/Drawable.h"
#include "Entity/Movable.h"

class Actor : public MovableEntity, public DrawableEntity {
 public:
  Actor(const sf::Texture& texture,
        std::unique_ptr<AbstractSpriteSheet> spriteSheet)
      : DrawableEntity(texture, std::move(spriteSheet)) {}
  Actor(const sf::Texture& texture,
        std::unique_ptr<AbstractSpriteSheet> spriteSheet,
        const std::vector<int>& frames, float frameRate = 1.f,
        int startFrameIndex = 0)
      : DrawableEntity(texture, std::move(spriteSheet), frames, frameRate,
                       startFrameIndex) {}

  Actor(const sf::Texture& texture) : DrawableEntity(texture) {}
  Actor(const sf::Texture& texture, const sf::IntRect& rect)
      : DrawableEntity(texture, rect) {}

  void update(sf::Time dt) override {
    MovableEntity::update(dt);
    DrawableEntity::update(dt);
  }
};
