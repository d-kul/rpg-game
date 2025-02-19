#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const TileSet &tileSet)
    : sf::Sprite(tileSet, tileSet.getTileRect(0)) {}

AnimatedSprite::AnimatedSprite(const TileSet &tileSet, const int *frames,
                               int framesSize, int startFrameIndex)
    : sf::Sprite(tileSet),
      currentFrameIndex(startFrameIndex),
      frames(frames),
      framesSize(framesSize) {}

void AnimatedSprite::setTileSet(const TileSet &tileSet) {
  sf::Sprite::setTexture(tileSet);
}

void AnimatedSprite::setFrames(const int *frames, int framesSize,
                               int startFrameIndex) {
  this->frames = frames;
  this->framesSize = framesSize;
  currentFrameIndex = startFrameIndex;
  if (framesSize != 0) {
    setTextureRect(getTileSet().getTileRect(frames[startFrameIndex]));
  }
  elapsedTime = sf::Time::Zero;
}

void AnimatedSprite::setAnimationFrequency(sf::Time animationFrequency) {
  this->animationFrequency = animationFrequency;
}

const TileSet &AnimatedSprite::getTileSet() const {
  return static_cast<const TileSet &>(getTexture());
}

void AnimatedSprite::update(sf::Time dt) {
  if ((elapsedTime += dt) > animationFrequency) {
    elapsedTime = sf::Time::Zero;
    if (framesSize == 1) return;
    if (++currentFrameIndex == framesSize) {
      currentFrameIndex = 0;
    }
    setTextureRect(getTileSet().getTileRect(frames[currentFrameIndex]));
  }
}
