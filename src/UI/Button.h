#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Core/Signal.h"
#include "UI/Element.h"

class Button : public UIElement {
 private:
  using sig_t = Signal<>;

 public:
  // Constructors, destructor
  Button(const sf::Font& font, sf::Vector2f size = {}, sf::String text = "",
         unsigned int textSize = 30, sf::Color textColor = sf::Color::Black,
         sf::Color idleColor = sf::Color::White,
         sf::Color hoverColor = sf::Color{200u, 200u, 200u},
         sf::Color activeColor = sf::Color{128u, 128u, 128u});
  ~Button();

  // Signals
  sig_t& onMousePressed();
  sig_t& onMouseReleased();
  sig_t& onClick();

  // Functionality
  void handleEvent(sf::Event event) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  // Members
  sf::RectangleShape shape;
  sf::Text text;
  sf::Color idleColor, hoverColor, activeColor;
  sf::Mouse::Button button = sf::Mouse::Button::Left;

 private:
  // Helpers
  bool pointInside(sf::Vector2i point);

  // Internal state
  bool pressed = false;

  // Signals
  sig_t onMousePressed_sig, onMouseReleased_sig, onClick_sig;
};
