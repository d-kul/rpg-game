#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>

#include "Manager/Event.h"
#include "UI/Element.h"

class UIManager {
 public:
  UIManager();
  void init();
  void handleEvent(sf::Event event);
  void setActiveState(std::string s = {});
  bool hasActiveState();
  void render();

 public:
  std::unordered_map<std::string, std::unique_ptr<UIElement>> states;

 private:
  sf::RenderWindow& window;
  EventManager& eventManager;
  sf::View view;
  UIElement* activeState = nullptr;
  std::string activeStateName;
};
