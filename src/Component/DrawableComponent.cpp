#include "DrawableComponent.h"

DrawableComponent::DrawableComponent() : index(getIndex()) {}

std::size_t DrawableComponent::getIndex() {
  static std::size_t index_ = 0;
  return index_++;
}

bool operator<(const DrawableComponent& a, const DrawableComponent& b) {
  return a.index < b.index;
}
