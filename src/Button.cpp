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

  auto& dispatcher = Game::getDispatcher();
  dispatcher.sink<sf::Event::MouseMoved>()
      .connect<&Button::onMouseMovedListener>(*this);
  dispatcher.sink<sf::Event::MouseButtonPressed>()
      .connect<&Button::onMouseButtonPressedListener>(*this);
  dispatcher.sink<sf::Event::MouseButtonReleased>()
      .connect<&Button::onMouseButtonReleasedListener>(*this);
}

Button::~Button() {
  auto& dispatcher = Game::getDispatcher();
  dispatcher.sink<sf::Event::MouseMoved>()
      .disconnect<&Button::onMouseMovedListener>(*this);
  dispatcher.sink<sf::Event::MouseButtonPressed>()
      .disconnect<&Button::onMouseButtonPressedListener>(*this);
  dispatcher.sink<sf::Event::MouseButtonReleased>()
      .disconnect<&Button::onMouseButtonReleasedListener>(*this);
}

// Sinks
entt::sink<Button::sig_t> Button::onMousePressed() {
  return onMousePressedSignal;
}

entt::sink<Button::sig_t> Button::onMouseReleased() {
  return onMouseReleasedSignal;
}

entt::sink<Button::sig_t> Button::onClick() { return onClickSignal; }

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
    onMousePressedSignal.publish();
    shape.setFillColor(activeColor);
    pressed = true;
  }
}

void Button::onMouseButtonReleasedListener(
    sf::Event::MouseButtonReleased mouseButtonReleased) {
  if (pointInside(mouseButtonReleased.position)) {
    onMouseReleasedSignal.publish();
    if (pressed) {
      onClickSignal.publish();
    }
    pressed = false;
    shape.setFillColor(hoverColor);
  }
}

// Helpers
bool Button::pointInside(sf::Vector2i point) {
  return getLocalBounds().contains(getTransform().getInverse().transformPoint(
      Game::getWindow().mapPixelToCoords(point)));
}
