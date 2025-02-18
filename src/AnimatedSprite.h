#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <vector>

#include "Resource/TileSet.h"

class AnimatedSprite : public sf::Sprite {
 public:
  static constexpr sf::Time DEFAULT_ANIMATION_FREQUENCY = sf::seconds(0.25f);

 public:
  AnimatedSprite(const TileSet &tileSet);

  AnimatedSprite(const TileSet &tileSet, std::vector<int> frames,
                 int startingFrame = 0);

  void setTileSet(const TileSet &tileSet);

  void setFrames(std::vector<int> frames, int startingFrame = 0);

  void setAnimationFrequency(sf::Time animationFrequency);

  const TileSet &getTileSet() const;

  void setTexture(const sf::Texture &texture, bool resetRect = false) = delete;

  void update(sf::Time dt);

 private:
  sf::Time elapsedTime = sf::Time::Zero;
  sf::Time animationFrequency = DEFAULT_ANIMATION_FREQUENCY;
  int currentFrame;
  std::vector<int> frames;
};
