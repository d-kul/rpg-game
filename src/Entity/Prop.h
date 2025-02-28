#pragma once

#include "Entity/Animated.h"
#include "Entity/Interactible.h"

class Prop : public AnimatedEntity, public InteractibleEntity {
 public:
  Prop(const TileSet& tileset, const std::vector<int>& frames = {0},
       float frameRate = 1.f, int startFrameIndex = 0,
       std::unique_ptr<AbstractAction> action = {})
      : AnimatedEntity(tileset, frames, frameRate, startFrameIndex),
        InteractibleEntity(std::move(action)) {}

  Prop(const TileSet& tileset, int frame = 0,
       std::unique_ptr<AbstractAction> action = {})
      : AnimatedEntity(tileset, {frame}),
        InteractibleEntity(std::move(action)) {}
};
