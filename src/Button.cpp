#include "Button.h"

#include "Game.h"

// Constructor, destructor
Button::Button(sf::Vector2f size, const sf::Font& font, sf::String text,
               unsigned int textSize, sf::Color textColor, sf::Color idleColor,
               sf::Color hoverColor, sf::Color activeColor)
    : shape(size),
      text(font, text, textSize),
      idleColor(idleColor),
      hoverColor(hoverColor),
      activeColor(activeColor) {
  shape.setFillColor(idleColor);
  this->text.setFillColor(textColor);
  this->text.setOrigin(this->text.getLocalBounds().getCenter());
  this->text.setPosition(shape.getLocalBounds().getCenter());
}

Button::~Button() {}

// Builder methods
Button& Button::setSize(sf::Vector2f size) {
  shape.setSize(size);
  return *this;
}

Button& Button::setFont(const sf::Font& font) {
  text.setFont(font);
  return *this;
}

Button& Button::setText(const sf::String& text) {
  this->text.setString(text);
  return *this;
}

Button& Button::setTextSize(unsigned int textSize) {
  text.setCharacterSize(textSize);
  return *this;
}

Button& Button::setTextColor(sf::Color color) {
  text.setFillColor(color);
  return *this;
}

Button& Button::setIdleColor(sf::Color color) {
  idleColor = color;
  return *this;
}

Button& Button::setHoverColor(sf::Color color) {
  hoverColor = color;
  return *this;
}

Button& Button::setActiveColor(sf::Color color) {
  activeColor = color;
  return *this;
}

Button& Button::setOnMousePressed(Button::hook_t hook) {
  onMousePressed_ = hook;
  return *this;
}

Button& Button::setOnMouseReleased(Button::hook_t hook) {
  onMouseReleased_ = hook;
  return *this;
}

Button& Button::setOnClick(Button::hook_t hook) {
  onClick_ = hook;
  return *this;
}

// Functionality
void Button::onEvent(sf::Event event) {
  if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
    if (pointInside(mouseMoved->position)) {
      shape.setFillColor(pressed ? activeColor : hoverColor);
    } else {
      pressed = false;
      shape.setFillColor(idleColor);
    }
  }
  if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (pointInside(mousePressed->position)) {
      if (onMousePressed_) onMousePressed_();
      shape.setFillColor(activeColor);
      pressed = true;
    }
  }
  if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>();
      mouseReleased && pointInside(mouseReleased->position)) {
    if (pointInside(mouseReleased->position)) {
      if (onMouseReleased_) onMouseReleased_();
      if (pressed && onClick_) {
        onClick_();
      }
      pressed = false;
      shape.setFillColor(hoverColor);
    }
  }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
  target.draw(text, states);
}

sf::FloatRect Button::getLocalBounds() { return shape.getLocalBounds(); }

sf::FloatRect Button::getGlobalBounds() {
  return getTransform().transformRect(shape.getLocalBounds());
}

// Helpers
bool Button::pointInside(sf::Vector2i point) {
  return getLocalBounds().contains(getTransform().getInverse().transformPoint(
      Game::getWindow().mapPixelToCoords(point)));
}
