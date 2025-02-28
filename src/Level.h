#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <list>

#include "Common/Data/Level.h"
#include "Entity/Background.h"
#include "Entity/Collider.h"
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
  void loadBackground(std::optional<LevelData::Background>& data);
  void loadTilemap(LevelData::Tilemap& data);
  void loadEntities(LevelData::Tilemap& tilemap, std::vector<EntityData>& data);

 private:
  ResourceManager& resourceManager;
  std::vector<std::shared_ptr<void>> resources;
  Background background;
  TileMap tilemap;
  std::list<std::unique_ptr<Entity>> entities;
  std::list<std::unique_ptr<Collider>> colliders;

  bool followPlayer = true;
  Player* player;
  std::unique_ptr<AbstractAction> activeAction;
};
