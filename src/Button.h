#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <entt/entt.hpp>

class Button : public sf::Transformable, public sf::Drawable {
 private:
  using sig_t = entt::sigh<void()>;

 public:
  // Constructor, destructor
  Button(sf::Vector2f size, const sf::Font& font, sf::String text = "",
         unsigned int textSize = 30, sf::Color textColor = sf::Color::Black,
         sf::Color idleColor = sf::Color::White,
         sf::Color hoverColor = sf::Color{200u, 200u, 200u},
         sf::Color activeColor = sf::Color{128u, 128u, 128u});
  ~Button();

  // Sinks
  entt::sink<sig_t> onMousePressed();
  entt::sink<sig_t> onMouseReleased();
  entt::sink<sig_t> onClick();

  // Functionality
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  sf::FloatRect getLocalBounds();
  sf::FloatRect getGlobalBounds();

 private:
  // Listeners
  void onMouseMovedListener(sf::Event::MouseMoved mouseMoved);
  void onMouseButtonPressedListener(
      sf::Event::MouseButtonPressed mouseButtonPressed);
  void onMouseButtonReleasedListener(
      sf::Event::MouseButtonReleased mouseButtonReleased);

  // Helpers
  bool pointInside(sf::Vector2i point);

  // Members
  bool pressed = false;
  sf::RectangleShape shape;
  sf::Text text;
  sf::Color idleColor, hoverColor, activeColor;

  // Signals
  sig_t onMousePressedSignal, onMouseReleasedSignal, onClickSignal;
};
