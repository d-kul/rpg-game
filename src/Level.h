#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <list>

#include "Common/Data/Level.h"
#include "Entity/Background.h"
#include "Entity/Collider.h"
#include "Entity/Player.h"
#include "Entity/TileMap.h"
#include "Game.h"

class Level {
 public:
  Level(Game& game);

  void loadFromFile(const std::filesystem::path& filename);
  void unload();

  void update(sf::Time dt);
  void render(sf::RenderWindow& window);

 private:
  void loadBackground(std::optional<LevelData::Background>& data);
  void loadTilemap(LevelData::Tilemap& data);
  void loadEntities(LevelData::Tilemap& tilemap, std::vector<EntityData>& data);

 private:
  Game& game;
  std::vector<std::shared_ptr<void>> resources;
  Background background;
  TileMap tilemap;
  std::list<std::unique_ptr<Entity>> entities;
  std::list<std::unique_ptr<Collider>> colliders;

  bool followPlayer = true;
  Player* player;
  Action* activeAction = nullptr;
  // TODO(des): store actions somewhere?
};
