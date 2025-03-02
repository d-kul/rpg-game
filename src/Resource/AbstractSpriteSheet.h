#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

class AbstractSpriteSheet {
 public:
  AbstractSpriteSheet() : texture(nullptr) {}
  AbstractSpriteSheet(const sf::Texture& texture) : texture(&texture) {}

  void setTexture(const sf::Texture& texture) { this->texture = &texture; }
  const sf::Texture& getTexture() const { return *texture; }

  virtual ~AbstractSpriteSheet() {}

  virtual sf::IntRect getRect(int index) const = 0;
  virtual unsigned getRectAmount() const = 0;

 protected:
  const sf::Texture* texture;
};
