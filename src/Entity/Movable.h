#pragma once

#include <SFML/System/Vector2.hpp>

#include "Entity.h"

class MovableEntity : public virtual Entity {
 public:
  void setDestination(sf::Vector2f destination);
  void update(sf::Time dt) override;

 protected:
  virtual void onSetDestination() {}
  virtual void onStop() {}
  virtual void onMove() {}

  sf::Vector2f destination = getPosition();
  std::optional<sf::Vector2f> nextDestination;
  float speed = 0.f;
  bool positionSnapped = false;
};
