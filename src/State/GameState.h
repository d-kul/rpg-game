#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <optional>

#include "Level.h"
#include "State.h"

class GameState : public State {
 private:
  // Lifetime management
  void loadResources();
  void loadLevel();

 public:
  GameState(Game& game);

  // State lifetime
  void enter() override;
  void exit() override;

  // Functionality
  void update(sf::Time dt) override;
  void render() override;

  // Listeners
  void onKeyPressed(sf::Event::KeyPressed keyPressed);
  void onKeyReleased(sf::Event::KeyReleased keyReleased);

 private:
  void initUI();

  void initImageFrame();
  void initTextFrame();
  void initMenuFrame();

  void setImageFrameActive(bool active);
  void setTextFrameActive(bool active);
  void setMenuFrameActive(bool active);

  void loadNextLevel(const std::filesystem::path& filename);

  friend class LevelAction;

 private:
  // Resources
  std::shared_ptr<sf::Font> font;
  std::shared_ptr<sf::Font> mono_font;
  std::shared_ptr<sf::Music> music;

  Level level;
  std::optional<std::filesystem::path> next_level;

  UIElement* mainFrame;
  UIElement* menuFrame;
  UIElement* textFrame;
  UIElement* imageFrame;

  ConnectionGuard onKeyPressed_cg, onKeyReleased_cg;
};
