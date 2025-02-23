#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <fstream>
#include <list>

#include "Entity/Background.h"
#include "Entity/Player.h"
#include "Entity/TileMap.h"
#include "Manager/Resource.h"

class Level {
 public:
  Level();

  void loadFromFile(const std::filesystem::path& filename);
  void unload();

  void update(sf::Time dt);
  void render(sf::RenderWindow& window);

 private:
  static void skipLine(std::ifstream& in);
  void loadBackground(std::ifstream& in);
  void loadTilemap(std::ifstream& in);
  void loadPlayer(std::ifstream& in);
  void loadEntities(std::ifstream& in);

 private:
  ResourceManager& resourceManager;
  std::vector<std::shared_ptr<void>> resources;
  Background background;
  TileMap tilemap;
  std::unique_ptr<Player> player;
  std::list<std::unique_ptr<Entity>> entities;
  std::list<std::unique_ptr<Collider>> colliders;

  friend class GameState;  // TODO: remove it later
};
