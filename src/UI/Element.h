#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <vector>

class UIElement : public sf::Transformable, public sf::Drawable {
 public:
  virtual ~UIElement() {}

  virtual void handleEvent(sf::Event event) {}
  void addChild(std::unique_ptr<UIElement>&& child);

 protected:
  void handleChildrenEvent(sf::Event event);
  void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::Transform getGlobalTransform();

 protected:
  UIElement* parent = nullptr;
  std::vector<std::unique_ptr<UIElement>> children;
};
