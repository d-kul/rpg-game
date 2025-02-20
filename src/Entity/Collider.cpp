#include "Collider.h"

#include "Game.h"

Collider::Collider() : manager(Game::getColliderManager()) {
  manager.addCollider(*this);
}

Collider::~Collider() {
  manager.removeCollider(*this);
}
