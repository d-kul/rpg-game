#pragma once

#include <SFML/Graphics.hpp>

class State {
 private:
  bool quit = false;

 public:
  State();
  virtual ~State();

  bool isQuit();
  void startQuit();

  virtual void endState() = 0;
  virtual void onEvent(const sf::Event& event);
  virtual void update(sf::Time dt) = 0;
  virtual void render(sf::RenderTarget& target) = 0;
};
