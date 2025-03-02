#pragma once

#include <vector>

#include "Resource/AbstractSpriteSheet.h"

class SpriteSheet : public AbstractSpriteSheet {
 public:
  SpriteSheet() = default;

  SpriteSheet(const sf::Texture& texture, std::vector<sf::IntRect> rects = {})
      : AbstractSpriteSheet(texture), rects(std::move(rects)) {}

  sf::IntRect getRect(int index) const override { return rects.at(index); }

  unsigned getRectAmount() const override { return rects.size(); }

  std::vector<sf::IntRect> rects;
};
