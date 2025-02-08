#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <stack>

#include "State.h"

class MainMenuState : public State {
 private:
  // Initialization
  void initKeybinds();

 public:
  // Constructors, destructor
  MainMenuState(std::map<std::string, sf::Keyboard::Key>& keybinds,
                sf::Vector2f window_size,
                std::stack<std::unique_ptr<State>>& states);
  virtual ~MainMenuState();

  // Functionality
  void endState() override;
  void onEvent(const sf::Event& event) override;
  void update(sf::Time dt) override;
  void render(sf::RenderTarget& target) override;

 private:
  // Members
  sf::Texture background_texture{"resources/pearto.png"};
  sf::RectangleShape background;
  std::stack<std::unique_ptr<State>>& states;
  const sf::Font font{"resources/papyrus.ttf"};
  sf::Text text{font,
                "Enter - start game\n"
                "Escape - quit\n"
                "WASD - move\n"
                "B - vine boom",
                40};
};
