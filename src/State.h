#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

class State {
 private:
  bool quit = false;
  std::vector<sf::Texture> textures;
 
 protected:
  std::map<std::string, sf::Keyboard::Key>& keybinds;

 public:
  State(std::map<std::string, sf::Keyboard::Key>& keybinds);
  virtual ~State();

  bool isQuit();
  void startQuit();

  virtual void endState() = 0;
  virtual void onEvent(const sf::Event& event);
  virtual void update(sf::Time dt) = 0;
  virtual void render(sf::RenderTarget& target) = 0;
};
