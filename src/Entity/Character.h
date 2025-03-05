#pragma once

#include "Entity/Actor.h"
#include "Entity/Interactible.h"

class Character : public Actor, public InteractibleEntity {
 public:
  Character(const sf::Texture& texture, InteractibleManager& manager,
            std::unique_ptr<AbstractSpriteSheet> spriteSheet,
            Action* action = nullptr)
      : Actor(texture, std::move(spriteSheet)),
        InteractibleEntity(manager, action) {}
  Character(const sf::Texture& texture, InteractibleManager& manager,
            std::unique_ptr<AbstractSpriteSheet> spriteSheet,
            const std::vector<int>& frames, float frameRate = 1.f,
            int startFrameIndex = 0, Action* action = nullptr)
      : Actor(texture, std::move(spriteSheet), frames, frameRate,
              startFrameIndex),
        InteractibleEntity(manager, action) {}

  Character(const sf::Texture& texture, InteractibleManager& manager,
            Action* action = nullptr)
      : Actor(texture), InteractibleEntity(manager, action) {}
  Character(const sf::Texture& texture, InteractibleManager& manager,
            const sf::IntRect& rect, Action* action = nullptr)
      : Actor(texture, rect), InteractibleEntity(manager, action) {}
};
