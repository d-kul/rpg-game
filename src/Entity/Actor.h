#pragma once

#include "Entity/Animated.h"
#include "Entity/Movable.h"

class Actor : public MovableEntity, public AnimatedEntity {
 public:
  Actor(const TileSet& tileset, const std::vector<int>& frames = {0},
        float frameRate = 1.f, int startFrameIndex = 0)
      : AnimatedEntity(tileset, frames, frameRate, startFrameIndex) {}

  void update(sf::Time dt) override {
    MovableEntity::update(dt);
    AnimatedEntity::update(dt);
  }
};
