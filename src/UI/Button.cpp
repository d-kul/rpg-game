#include "Button.h"

#include <SFML/Window/Event.hpp>

#include "Game.h"

// Constructor, destructor
Button::Button(const sf::Font& font, sf::Vector2f size, sf::String text,
               unsigned int textSize, sf::Color textColor, sf::Color idleColor,
               sf::Color hoverColor, sf::Color activeColor)
    : window(Game::getWindow()),
      shape(size),
      text(font, text, textSize),
      idleColor(idleColor),
      hoverColor(hoverColor),
      activeColor(activeColor),
      onMouseMoved_cg(Game::getEventHandler().bind<sf::Event::MouseMoved>(
          &Button::onMouseMoved_l, this)),
      onMousePressed_cg(
          Game::getEventHandler().bind<sf::Event::MouseButtonPressed>(
              &Button::onMousePressed_l, this)),
      onMouseReleased_cg(
          Game::getEventHandler().bind<sf::Event::MouseButtonReleased>(
              &Button::onMouseReleased_l, this)) {
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
void Button::onMouseMoved_l(sf::Event::MouseMoved mouseMoved) {
  if (pointInside(mouseMoved.position)) {
    shape.setFillColor(pressed ? activeColor : hoverColor);
  } else {
    pressed = false;
    shape.setFillColor(idleColor);
  }
}

void Button::onMousePressed_l(
    sf::Event::MouseButtonPressed mouseButtonPressed) {
  if (pointInside(mouseButtonPressed.position)) {
    onMousePressed_sig();
    shape.setFillColor(activeColor);
    pressed = true;
  }
}

void Button::onMouseReleased_l(
    sf::Event::MouseButtonReleased mouseButtonReleased) {
  if (pointInside(mouseButtonReleased.position)) {
    onMouseReleased_sig();
    if (pressed) {
      onClick_sig();
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
