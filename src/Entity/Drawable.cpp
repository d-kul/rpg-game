#include "Drawable.h"

DrawableEntity::DrawableEntity(const AbstractSpriteSheet& spriteSheet)
    : sprite(spriteSheet.getTexture()),
      spriteSheet(&spriteSheet),
      animated(true),
      frames(nullptr) {}

DrawableEntity::DrawableEntity(const AbstractSpriteSheet& spriteSheet,
                               const std::vector<int>& frames, float frameRate,
                               int startFrameIndex)
    : sprite(spriteSheet.getTexture()),
      spriteSheet(&spriteSheet),
      animated(true),
      frames(&frames),
      frameIndex(startFrameIndex) {
  assert(!frames.empty() && "frame list should not be empty");
  assert(frameRate > 0 && "frame rate should be strictly positive");
  setFrames(frames, startFrameIndex);
  setFrameRate(frameRate);
}

DrawableEntity::DrawableEntity(const sf::Texture& texture)
    : sprite(texture), animated(false) {}
DrawableEntity::DrawableEntity(const sf::Texture& texture,
                               const sf::IntRect& rect)
    : sprite(texture, rect), animated(false) {}

sf::FloatRect DrawableEntity::getGlobalBounds() {
  return getTransform().transformRect(sprite.getGlobalBounds());
}

sf::FloatRect DrawableEntity::getLocalBounds() {
  return sprite.getGlobalBounds();
}

void DrawableEntity::setFrames(const std::vector<int>& frames,
                               int startFrameIndex) {
  assert(animated);
  this->frames = &frames;
  setFrame(startFrameIndex);
}

void DrawableEntity::setFrameRate(float frameRate) {
  assert(animated);
  frameInterval = sf::seconds(1.f / frameRate);
  elapsed = sf::Time::Zero;
}

void DrawableEntity::update(sf::Time dt) {
  if (!animated || !frames || frames->size() == 1) return;
  elapsed += dt;
  while (elapsed >= frameInterval) {
    elapsed -= frameInterval;
    setFrame(frameIndex + 1 < frames->size() ? frameIndex + 1 : 0);
  }
}

void DrawableEntity::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const {
  if (animated && !frames) return;
  states.transform *= getTransform();
  target.draw(sprite, states);
}

void DrawableEntity::setFrame(int index) {
  frameIndex = index;
  sprite.setTextureRect(spriteSheet->getRect(frames->at(frameIndex)));
}
