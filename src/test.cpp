#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

int main() {
  // Create the main window
  sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

  // Load a sprite to display
  const sf::Texture texture("resources/images/teto_plush.png");
  sf::Sprite sprite(texture);

  // Create a graphical text to display
  const sf::Font font("resources/fonts/papyrus.ttf");
  sf::Text text(font, "Hello SFML", 50);

  // Load a music to play
  sf::Music music("resources/music/teto scatman.ogg");

  // Play the music
  music.play();

  // Start the game loop
  while (window.isOpen()) {
    // Process events
    while (const std::optional event = window.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>()) window.close();
      if (auto keyReleased = event->getIf<sf::Event::KeyReleased>()) {
        if (keyReleased->code == sf::Keyboard::Key::F12) {
          window.create(sf::VideoMode({800, 600}), "SFML window",
                        sf::Style::Default, sf::State::Fullscreen);
        }
        if (keyReleased->code == sf::Keyboard::Key::F11) {
          window.create(sf::VideoMode({1920, 1080}), "SFML window",
                        sf::Style::Default, sf::State::Fullscreen);
        }
        if (keyReleased->code == sf::Keyboard::Key::F10) {
          window.create(sf::VideoMode({800, 600}), "SFML window");
        }
        if (keyReleased->code == sf::Keyboard::Key::Escape) {
          window.close();
        }
      }
    }

    // Clear screen
    window.clear();

    // Draw the sprite
    window.draw(sprite);

    // Draw the string
    window.draw(text);

    // Update the window
    window.display();
  }
}
