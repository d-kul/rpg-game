#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>

#include "UI/Element.h"

class UIManager {
 public:
  using states_t = std::unordered_map<std::string, std::unique_ptr<UIElement>>;

 public:
  UIManager(sf::RenderWindow& window);
  void init(sf::Vector2u windowSize);
  void handleEvent(sf::Event event);
  void setActiveState(const std::string& s = {});
  bool hasActiveState();
  const std::string& getActiveStateName();
  void setView(sf::View view);
  void render();

 public:
  states_t states;

 private:
  sf::RenderWindow& window;
  sf::View view;

  UIElement* activeState = nullptr;
  const std::string* activeStateName = nullptr;
};
