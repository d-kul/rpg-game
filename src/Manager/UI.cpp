#include "UI.h"

#include "Game.h"

UIManager::UIManager()
    : window(Game::getWindow()), eventManager(Game::getEventManager()) {}

void UIManager::init() {
  auto& window_size = Game::getWindowSize();
  view.setCenter(sf::Vector2f{window_size} / 2.f);
  view.setSize(sf::Vector2f{window_size});
}

void UIManager::handleEvent(sf::Event event) {
  if (activeState) activeState->handleEvent(event);
}

void UIManager::unsetActiveState() { activeState = nullptr; }

void UIManager::setActiveState(std::string s) {
  auto it = states.find(s);
  if (it != states.end()) activeState = it->second.get();
}

bool UIManager::hasActiveState() { return activeState != nullptr; }

void UIManager::render() {
  if (!activeState) return;
  auto prevView = window.getView();
  window.setView(view);
  window.draw(*activeState);
  window.setView(prevView);
}
