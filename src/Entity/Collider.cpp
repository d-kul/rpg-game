#include "Collider.h"

Collider::Collider(ColliderManager& manager) : manager(manager) {
  manager.addCollider(*this);
}

Collider::~Collider() { manager.removeCollider(*this); }
