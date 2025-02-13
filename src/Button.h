#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Button : public sf::Transformable, public sf::Drawable {
 private:
  using hook_t = std::function<void()>;

 public:
  // Constructor, destructor
  Button(sf::Vector2f size, const sf::Font& font, sf::String text = "",
         unsigned int textSize = 30, sf::Color textColor = sf::Color::Black,
         sf::Color idleColor = sf::Color::White,
         sf::Color hoverColor = sf::Color{200u, 200u, 200u},
         sf::Color activeColor = sf::Color{128u, 128u, 128u});
  ~Button() = default;

  // Builder methods
  Button& setSize(sf::Vector2f size);
  Button& setFont(const sf::Font& font);
  Button& setText(const sf::String& text);
  Button& setTextSize(unsigned int textSize);
  Button& setTextColor(sf::Color color);
  Button& setIdleColor(sf::Color color);
  Button& setHoverColor(sf::Color color);
  Button& setActiveColor(sf::Color color);
  Button& setOnMousePressed(hook_t hook);
  Button& setOnMouseReleased(hook_t hook);
  Button& setOnClick(hook_t hook);

  // Functionality
  void handleEvent(sf::Event event);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  sf::FloatRect getLocalBounds();
  sf::FloatRect getGlobalBounds();

 private:
  // Helpers
  bool pointInside(sf::Vector2i point);

  // Members
  bool pressed = false;
  sf::RectangleShape shape;
  sf::Text text;
  sf::Color idleColor, hoverColor, activeColor;

  // Hooks
  hook_t onMousePressed_, onMouseReleased_, onClick_;
};
