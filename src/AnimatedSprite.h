#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include "Resource/TileSet.h"

class AnimatedSprite : public sf::Sprite {
 public:
  AnimatedSprite(const TileSet &tileSet);

  AnimatedSprite(const TileSet &tileSet, const int *frames, int framesSize,
                 int startFrameIndex = 0);

  void setTileSet(const TileSet &tileSet);

  void setFrames(const int *frames, int framesSize, int startFrameIndex = 0);

  void setAnimationFrequency(sf::Time animationFrequency);

  const TileSet &getTileSet() const;

  void setTexture(const sf::Texture &texture, bool resetRect = false) = delete;

  void update(sf::Time dt);

 private:
  sf::Time elapsedTime = sf::Time::Zero;
  sf::Time animationFrequency = sf::seconds(0.25f);
  int currentFrameIndex;
  const int *frames;
  int framesSize = 0;
};
