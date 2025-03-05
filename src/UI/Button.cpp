#include "Button.h"

#include <SFML/Window/Event.hpp>

// Constructor, destructor
Button::Button(const sf::Font& font, sf::Vector2f size, sf::String text,
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

// Signals
Button::sig_t& Button::onMousePressed() { return onMousePressed_sig; }

Button::sig_t& Button::onMouseReleased() { return onMouseReleased_sig; }

Button::sig_t& Button::onClick() { return onClick_sig; }

// Functionality

bool Button::handleEvent(sf::Event event) {
  if (auto mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
    if (pointInside(shape.getLocalBounds(), mouseMoved->position)) {
      shape.setFillColor(pressed ? activeColor : hoverColor);
    } else {
      pressed = false;
      shape.setFillColor(idleColor);
    }
  } else if (auto mouseButtonPressed =
                 event.getIf<sf::Event::MouseButtonPressed>();
             mouseButtonPressed && mouseButtonPressed->button == button) {
    if (pointInside(shape.getLocalBounds(), mouseButtonPressed->position)) {
      onMousePressed_sig();
      shape.setFillColor(activeColor);
      pressed = true;
      return true;
    }
  } else if (auto mouseButtonReleased =
                 event.getIf<sf::Event::MouseButtonReleased>();
             mouseButtonReleased && mouseButtonReleased->button == button) {
    if (pointInside(shape.getLocalBounds(), mouseButtonReleased->position)) {
      onMouseReleased_sig();
      if (pressed) {
        onClick_sig();
      }
      pressed = false;
      shape.setFillColor(hoverColor);
      return true;
    }
  }
  return false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape, states);
  target.draw(text, states);
}
