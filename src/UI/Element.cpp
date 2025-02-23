#include "Element.h"

#include <SFML/Graphics/RenderStates.hpp>

void UIElement::addChild(std::unique_ptr<UIElement>&& child) {
  child->parent = this;
  children.push_back(std::move(child));
}

void UIElement::handleChildrenEvent(sf::Event event) {
  for (auto& child : children) {
    child->handleEvent(event);
  }
}

sf::Transform UIElement::getGlobalTransform() {
  if (!parent) return getTransform();
  return getTransform() * parent->getGlobalTransform();
}

void UIElement::drawChildren(sf::RenderTarget& target,
                             sf::RenderStates states) const {
  for (auto& child : children) {
    child->draw(target, states);
  }
}
