#pragma once

#include "Entity/Actor.h"
#include "Entity/Interactible.h"

class Character : public Actor, public InteractibleEntity {
 public:
  Character(const sf::Texture& texture,
            std::unique_ptr<AbstractSpriteSheet> spriteSheet,
            std::unique_ptr<AbstractAction> action = {})
      : Actor(texture, std::move(spriteSheet)),
        InteractibleEntity(std::move(action)) {}
  Character(const sf::Texture& texture,
            std::unique_ptr<AbstractSpriteSheet> spriteSheet,
            const std::vector<int>& frames, float frameRate = 1.f,
            int startFrameIndex = 0,
            std::unique_ptr<AbstractAction> action = {})
      : Actor(texture, std::move(spriteSheet), frames, frameRate,
              startFrameIndex),
        InteractibleEntity(std::move(action)) {}

  Character(const sf::Texture& texture,
            std::unique_ptr<AbstractAction> action = {})
      : Actor(texture), InteractibleEntity(std::move(action)) {}
  Character(const sf::Texture& texture, const sf::IntRect& rect,
            std::unique_ptr<AbstractAction> action = {})
      : Actor(texture, rect), InteractibleEntity(std::move(action)) {}
};
