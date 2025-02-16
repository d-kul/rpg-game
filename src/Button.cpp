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

  // TODO: events...
  // auto& dispatcher = Game::getDispatcher();
  // dispatcher.sink<sf::Event::MouseMoved>()
  //     .connect<&Button::onMouseMovedListener>(*this);
  // dispatcher.sink<sf::Event::MouseButtonPressed>()
  //     .connect<&Button::onMouseButtonPressedListener>(*this);
  // dispatcher.sink<sf::Event::MouseButtonReleased>()
  //     .connect<&Button::onMouseButtonReleasedListener>(*this);
}

Button::~Button() {
  // TODO: events...
  // auto& dispatcher = Game::getDispatcher();
  // dispatcher.sink<sf::Event::MouseMoved>()
  //     .disconnect<&Button::onMouseMovedListener>(*this);
  // dispatcher.sink<sf::Event::MouseButtonPressed>()
  //     .disconnect<&Button::onMouseButtonPressedListener>(*this);
  // dispatcher.sink<sf::Event::MouseButtonReleased>()
  //     .disconnect<&Button::onMouseButtonReleasedListener>(*this);
}

// Signals
Button::sig_t& Button::onMousePressed() { return onMousePressedSignal; }

Button::sig_t& Button::onMouseReleased() { return onMouseReleasedSignal; }

Button::sig_t& Button::onClick() { return onClickSignal; }

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
    for (auto& f : onMousePressedSignal) f();
    shape.setFillColor(activeColor);
    pressed = true;
  }
}

void Button::onMouseButtonReleasedListener(
    sf::Event::MouseButtonReleased mouseButtonReleased) {
  if (pointInside(mouseButtonReleased.position)) {
    for (auto& f : onMouseReleasedSignal) f();
    if (pressed) {
      for (auto& f : onClickSignal) f();
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
