#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <vector>

#include "Entity.h"
#include "Resource/AbstractSpriteSheet.h"

class DrawableEntity : public virtual Entity {
 public:
  DrawableEntity(const sf::Texture& texture,
                 std::unique_ptr<AbstractSpriteSheet> spriteSheet);
  DrawableEntity(const sf::Texture& texture,
                 std::unique_ptr<AbstractSpriteSheet> spriteSheet,
                 const std::vector<int>& frames, float frameRate = 1.f,
                 int startFrameIndex = 0);

  DrawableEntity(const sf::Texture& texture);
  DrawableEntity(const sf::Texture& texture, const sf::IntRect& rect);

  sf::FloatRect getGlobalBounds();
  sf::FloatRect getLocalBounds();
  void setFrames(const std::vector<int>& frames, int startFrameIndex = 0);
  void setFrameRate(float frameRate);

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  void setFrame(int index);

 protected:
  sf::Sprite sprite;
  bool animated;
  std::unique_ptr<AbstractSpriteSheet> spriteSheet;
  const std::vector<int>* frames;
  sf::Time frameInterval;
  int frameIndex;
  sf::Time elapsed = sf::Time::Zero;
};
