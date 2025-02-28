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
  void loadAssets();

 public:
  // State lifetime
  void enter() override;
  void exit() override;

  // Functionality
  void update(sf::Time dt) override;
  void render() override;

  // Listeners
  void onKeyReleased(sf::Event::KeyReleased keyReleased);

 private:
  void loadNextLevel(const std::filesystem::path& filename);
  void initUI();

  friend class LevelAction;

 private:
  // Resources
  std::shared_ptr<sf::Font> font;
  std::shared_ptr<sf::Font> mono_font;
  std::shared_ptr<sf::Music> music;
  std::shared_ptr<sf::Texture> pearto_texture;

  Level level;
  std::optional<std::filesystem::path> next_level;

  ConnectionGuard onKeyReleased_cg;
};
