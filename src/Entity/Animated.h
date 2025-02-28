#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Entity.h"
#include "Resource/TileSet.h"

class AnimatedEntity : public virtual Entity {
 public:
  AnimatedEntity(const TileSet& tileset, const std::vector<int>& frames = {0},
                 float frameRate = 1.f, int startFrameIndex = 0);

  void setFrames(const std::vector<int>& frames, int startFrameIndex = 0);
  void setFrameRate(float frameRate);

  sf::Sprite& getSprite();

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  void setFrame(int index);

 protected:
  const TileSet* tileset;
  sf::Sprite sprite;
  const std::vector<int>* frames;
  sf::Time frameInterval;
  int frameIndex;
  sf::Time elapsed;
};
