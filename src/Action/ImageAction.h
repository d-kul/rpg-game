#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "../Action.h"
#include "Level.h"

class ImageAction : public Action {
 public:
  ImageAction(Level& level, sf::Texture& texture)
      : level(level), texture(texture) {}

  void start() override {
    level.setImageFrameActive(true);
    auto imageSize = sf::Vector2f{texture.getSize()};
    auto screenSize = sf::Vector2f{level.game.videoMode.size};
    float padding = 35.f; // TODO(des): ooohh magic numbers
    float scaling =
        std::min((std::min(screenSize.x, 700.f) - padding) / imageSize.x,
                 (std::min(screenSize.y, 700.f) - padding) / imageSize.y);
    imageSize *= scaling;
    level.imageFrameShape->setSize(imageSize);
    level.imageFrameShape->setOrigin(
        level.imageFrameShape->getGeometricCenter());
    level.imageFrameShape->setTexture(&texture, true);
    level.imageClosed = false;
  }

  bool update(sf::Time dt) override { return !level.imageClosed; }

  void end() override {
    level.setImageFrameActive(false);
    level.imageFrameShape->setTexture(nullptr);
  }

 private:
  Level& level;
  sf::Texture& texture;
};
