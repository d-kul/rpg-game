#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Clock.hpp>
#include <cmath>

int main() {
  auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "something cool");
  window.setFramerateLimit(144);

  sf::Texture bg_texture("test.jpg", false, sf::IntRect({0, 2}, {190, 162}));
  bg_texture.setRepeated(true);
  const unsigned width = 190;
  sf::Sprite bg_sprite(bg_texture);
  auto width_scaling = 162.f / 190.f;
  bg_sprite.setTextureRect(sf::IntRect(
      {0u, 0u}, {static_cast<int>(std::ceil(800.f / width_scaling)), 600u}));
  bg_sprite.setScale({width_scaling, 1.f});

  sf::Font font("papyrus.ttf");
  sf::Text text(font);
  text.setString("game design is my passion");
  text.setCharacterSize(40);
  text.setFillColor(sf::Color::Green);
  text.setOutlineColor(sf::Color::Blue);
  text.setOutlineThickness(3.f);
  text.setStyle(sf::Text::Bold);
  text.setPosition({80.f, 400.f});

  sf::Texture forg_texture("forg.jpg");
  sf::Sprite forg_sprite(forg_texture);
  forg_sprite.setScale({0.3f, 0.3f});
  forg_sprite.setPosition({520.f, 120.f});

  sf::Clock clock;
  while (window.isOpen()) {
    sf::Time elapsed = clock.restart();
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear();
    window.draw(bg_sprite);
    window.draw(text);
    window.draw(forg_sprite);
    window.display();
  }
}
