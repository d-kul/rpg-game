#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <fstream>
#include <list>

#include "Common/LevelData.h"
#include "Entity/Background.h"
#include "Entity/Collider.h"
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
  void loadBackground(std::optional<LevelData::BackgroundData>& data);
  void loadTilemap(LevelData::TilemapData& data);
  void loadEntities(std::vector<LevelData::EntityData>& data, Entity*& player,
                    sf::Vector2u& playerPos);

 private:
  ResourceManager& resourceManager;
  std::vector<std::shared_ptr<void>> resources;
  Background background;
  TileMap tilemap;
  std::list<std::unique_ptr<Entity>> entities;
  std::list<std::unique_ptr<Collider>> colliders;

  friend class GameState;  // TODO: remove it later
};
