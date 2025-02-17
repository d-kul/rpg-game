#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "Core/Signal.h"

class Button : public sf::Transformable, public sf::Drawable {
 private:
  using sig_t = Signal<>;

 public:
  // Constructors, destructor
  Button(const sf::Font& font, sf::Vector2f size = {},
         sf::String text = "", unsigned int textSize = 30,
         sf::Color textColor = sf::Color::Black,
         sf::Color idleColor = sf::Color::White,
         sf::Color hoverColor = sf::Color{200u, 200u, 200u},
         sf::Color activeColor = sf::Color{128u, 128u, 128u});
  ~Button();

  // Signals
  sig_t& onMousePressed();
  sig_t& onMouseReleased();
  sig_t& onClick();

  // Functionality
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  sf::FloatRect getLocalBounds();
  sf::FloatRect getGlobalBounds();

 private:
  // Listeners
  void onMouseMoved_l(sf::Event::MouseMoved mouseMoved);
  void onMousePressed_l(
      sf::Event::MouseButtonPressed mouseButtonPressed);
  void onMouseReleased_l(
      sf::Event::MouseButtonReleased mouseButtonReleased);

  // Helpers
  bool pointInside(sf::Vector2i point);

  // Bindings
  sf::RenderWindow& window;

  // Members
  bool pressed = false;
  sf::RectangleShape shape;
  sf::Text text;
  sf::Color idleColor, hoverColor, activeColor;

  // Signals
  sig_t onMousePressed_sig, onMouseReleased_sig, onClick_sig;

  ConnectionGuard onMouseMoved_cg;
  ConnectionGuard onMousePressed_cg;
  ConnectionGuard onMouseReleased_cg;
};
