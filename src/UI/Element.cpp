#include "Element.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

void UIElement::addChild(std::unique_ptr<UIElement> child) {
  child->parent = this;
  children.push_back(std::move(child));
}

const UIElement::children_t& UIElement::getChildren() const {
  return children;
}

bool UIElement::handleChildrenEvent(sf::Event event) {
  for (auto it = children.rbegin(); it != children.rend(); ++it) {
    auto& child = *it;
    bool res = child->handleEvent(event);
    if (res) return res;
  }
  return false;
}

void UIElement::drawChildren(sf::RenderTarget& target,
                             sf::RenderStates states) const {
  for (auto& child : children) {
    child->draw(target, states);
  }
}

sf::Transform UIElement::getGlobalTransform() const {
  if (!parent) return getTransform();
  return getTransform() * parent->getGlobalTransform();
}

bool UIElement::pointInside(sf::FloatRect localRect, sf::Vector2i point) const {
  return localRect.contains(
      getGlobalTransform().getInverse().transformPoint(sf::Vector2f{point}));
}
