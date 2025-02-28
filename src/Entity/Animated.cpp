#include "Animated.h"

AnimatedEntity::AnimatedEntity(const TileSet& tileset,
                               const std::vector<int>& frames, float frameRate,
                               int startFrameIndex)
    : tileset(&tileset), sprite(tileset) {
  assert(!frames.empty() && "frame list should not be empty");
  assert(frameRate > 0 && "frame rate should be strictly positive");
  setFrameRate(frameRate);
  setFrames(frames, startFrameIndex);
}

void AnimatedEntity::setFrames(const std::vector<int>& frames,
                               int startFrameIndex) {
  this->frames = &frames;
  setFrame(startFrameIndex);
}

void AnimatedEntity::setFrameRate(float frameRate) {
  frameInterval = sf::seconds(1.f / frameRate);
  elapsed = sf::Time::Zero;
}

sf::Sprite& AnimatedEntity::getSprite() { return sprite; }

void AnimatedEntity::update(sf::Time dt) {
  if (frames->size() == 1) return;
  elapsed += dt;
  while (elapsed > frameInterval) {
    elapsed -= frameInterval;
    setFrame(frameIndex + 1 < frames->size() ? frameIndex + 1 : 0);
  }
}

void AnimatedEntity::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite, states);
}

void AnimatedEntity::setFrame(int index) {
  frameIndex = index;
  sprite.setTextureRect(tileset->getTileRect(frames->at(frameIndex)));
}
