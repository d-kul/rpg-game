#include "Button.h"

// Constructor, destructor
Button::Button(sf::RenderWindow& window, const sf::Font& font,
               sf::Vector2f size, sf::String text, unsigned int textSize,
               sf::Color textColor, sf::Color idleColor, sf::Color hoverColor,
               sf::Color activeColor)
    : window(window),
      shape(size),
      text(font, text, textSize),
      idleColor(idleColor),
      hoverColor(hoverColor),
      activeColor(activeColor) {
  shape.setFillColor(idleColor);
  this->text.setFillColor(textColor);
  this->text.setOrigin(this->text.getLocalBounds().getCenter());
  this->text.setPosition(shape.getLocalBounds().getCenter());
}

Button::~Button() { unhookListeners(); }

// Signals
Button::sig_t& Button::onMousePressed() { return onMousePressedSignal; }

Button::sig_t& Button::onMouseReleased() { return onMouseReleasedSignal; }

Button::sig_t& Button::onClick() { return onClickSignal; }

void Button::hookListeners(EventHandler& eventHandler) {
  onMouseMovedConnection = eventHandler.sink<sf::Event::MouseMoved>().subscribe(
      std::bind(&Button::onMouseMovedListener, this, std::placeholders::_1));
  onMousePressedConnection =
      eventHandler.sink<sf::Event::MouseButtonPressed>().subscribe(std::bind(
          &Button::onMouseButtonPressedListener, this, std::placeholders::_1));
  onMouseReleasedConnection =
      eventHandler.sink<sf::Event::MouseButtonReleased>().subscribe(std::bind(
          &Button::onMouseButtonReleasedListener, this, std::placeholders::_1));
}

void Button::unhookListeners() {
  if (onMouseMovedConnection) onMouseMovedConnection.disconnect();
  if (onMousePressedConnection) onMousePressedConnection.disconnect();
  if (onMouseReleasedConnection) onMouseReleasedConnection.disconnect();
}

// Functionality
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
  target.draw(text, states);
}

sf::FloatRect Button::getLocalBounds() { return shape.getLocalBounds(); }

sf::FloatRect Button::getGlobalBounds() {
  return getTransform().transformRect(shape.getLocalBounds());
}

// Listeners
void Button::onMouseMovedListener(sf::Event::MouseMoved mouseMoved) {
  if (pointInside(mouseMoved.position)) {
    shape.setFillColor(pressed ? activeColor : hoverColor);
  } else {
    pressed = false;
    shape.setFillColor(idleColor);
  }
}

void Button::onMouseButtonPressedListener(
    sf::Event::MouseButtonPressed mouseButtonPressed) {
  if (pointInside(mouseButtonPressed.position)) {
    onMousePressedSignal();
    shape.setFillColor(activeColor);
    pressed = true;
  }
}

void Button::onMouseButtonReleasedListener(
    sf::Event::MouseButtonReleased mouseButtonReleased) {
  if (pointInside(mouseButtonReleased.position)) {
    onMouseReleasedSignal();
    if (pressed) {
      onClickSignal();
    }
    pressed = false;
    shape.setFillColor(hoverColor);
  }
}

// Helpers
bool Button::pointInside(sf::Vector2i point) {
  return getLocalBounds().contains(getTransform().getInverse().transformPoint(
      window.mapPixelToCoords(point)));
}
