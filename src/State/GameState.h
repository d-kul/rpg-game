#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

#include "Entity/Background.h"
#include "Entity/Player.h"
#include "Entity/TileMap.h"
#include "Entity/TileMapCollider.h"
#include "Entity/RectCollider.h"
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
  // Resources
  std::shared_ptr<sf::Texture> background_texture;
  std::shared_ptr<sf::Texture> pearto_texture;
  std::shared_ptr<sf::Font> font;
  std::shared_ptr<sf::Font> mono_font;
  std::shared_ptr<sf::Music> music;
  std::shared_ptr<TileSet> tileset;

  // Members
  Background background;
  std::unique_ptr<sf::Text> main_text;
  std::unique_ptr<sf::Text> sounds_text;
  std::unique_ptr<sf::Text> mouse_text;
  std::unique_ptr<Player> player;
  std::unique_ptr<Interactible> test_entity;
  RectCollider test_entity_collider;
  TileMap tilemap;
  TileMapCollider tilemap_collider;

  ConnectionGuard onKeyReleased_cg;
};
