#pragma once

#include "Entity/Actor.h"
#include "Entity/Interactible.h"

class Character : public Actor, public InteractibleEntity {
 public:
  Character(const TileSet& tileset, const std::vector<int>& frames = {0},
            float frameRate = 1.f, int startFrameIndex = 0,
            std::unique_ptr<AbstractAction> action = {})
      : Actor(tileset, frames, frameRate, startFrameIndex),
        InteractibleEntity(std::move(action)) {}

  Character(const TileSet& tileset, int frame = 0,
            std::unique_ptr<AbstractAction> action = {})
      : Actor(tileset, {frame}),
        InteractibleEntity(std::move(action)) {}
};

