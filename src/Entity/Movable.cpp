#include "Movable.h"

void MovableEntity::setDestination(sf::Vector2f destination) {
  this->destination = destination;
  onSetDestination();
}

void MovableEntity::update(sf::Time dt) {
  static constexpr float SNAP_RANGE = 0.05f;
  auto path_left = destination - getPosition();
  float step = speed * dt.asSeconds();

  if (path_left.length() < SNAP_RANGE ||
      step > path_left.length() + SNAP_RANGE) {
    if (!positionSnapped) {
      setPosition(destination);
      positionSnapped = true;
      if (!nextDestination) {
        onStop();
      }
    }
    if (nextDestination) {
      step -= path_left.length();
      setDestination(*nextDestination);
      path_left = destination - getPosition();
      nextDestination.reset();
    }
  }
  if (path_left.length() >= SNAP_RANGE &&
      step <= path_left.length() + SNAP_RANGE) {
    move(path_left.normalized() * step);
    positionSnapped = false;
    onMove();
  }
}
