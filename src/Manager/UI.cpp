#include "UI.h"

#include "Core/Logger.h"

UIManager::UIManager(sf::RenderWindow& window) : window(window) {}

void UIManager::init(sf::Vector2u windowSize) {
  view.setCenter(sf::Vector2f{windowSize} / 2.f);
  view.setSize(sf::Vector2f{windowSize});
}

void UIManager::handleEvent(sf::Event event) {
  if (activeState) activeState->handleEvent(event);
}

void UIManager::setActiveState(const std::string& s) {
  if (s.empty()) {
    activeState = nullptr;
    activeStateName = nullptr;
  } else if (auto it = states.find(s); it != states.end()) {
    activeState = it->second.get();
    activeStateName = &it->first;
  } else {
    ERROR("state \"", s, "\" has not been found");
  }
}

bool UIManager::hasActiveState() { return activeState != nullptr; }

const std::string& UIManager::getActiveStateName() { return *activeStateName; }

void UIManager::setView(sf::View view) { this->view = view; }

void UIManager::render() {
  if (!activeState) return;
  auto prevView = window.getView();
  window.setView(view);
  window.draw(*activeState);
  window.setView(prevView);
}
