#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <vector>

class UIElement : public sf::Transformable, public sf::Drawable {
 public:
  using children_t = std::vector<std::unique_ptr<UIElement>>;

 public:
  virtual ~UIElement() {}

  virtual bool handleEvent(sf::Event event) { return false; }
  void setActive(bool active);
  bool isActive() const;
  void addChild(std::unique_ptr<UIElement> child);
  const children_t& getChildren() const;

 protected:
  bool handleChildrenEvent(sf::Event event);
  void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::Transform getGlobalTransform() const;
  bool pointInside(sf::FloatRect localRect, sf::Vector2i point) const;

 protected:
  UIElement* parent = nullptr;
  bool active = true;
  children_t children;
};
