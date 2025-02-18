#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const TileSet &tileSet)
    : sf::Sprite(tileSet, tileSet.getTileRect(0)) {}

AnimatedSprite::AnimatedSprite(const TileSet &tileSet, std::vector<int> frames,
                               int startingFrame)
    : sf::Sprite(tileSet, tileSet.getTileRect(frames[startingFrame])),
      currentFrame(startingFrame),
      frames(frames) {}

void AnimatedSprite::setTileSet(const TileSet &tileSet) {
  sf::Sprite::setTexture(tileSet);
  setTextureRect(tileSet.getTileRect(0));
}

void AnimatedSprite::setFrames(std::vector<int> frames, int startingFrame) {
  this->frames = frames;
  currentFrame = startingFrame;
  setTextureRect(getTileSet().getTileRect(startingFrame));
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
    if (++currentFrame == frames.size()) {
      currentFrame = 0;
    }
    setTextureRect(getTileSet().getTileRect(frames[currentFrame]));
  }
}
