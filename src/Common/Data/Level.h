#pragma once

#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <iostream>
#include <optional>
#include <vector>

#include "Common/Data/Entity.h"

class LevelData {
 public:
   static constexpr const char* BACKGROUND_HEADER = "[Background]";
   static constexpr const char* TILEMAP_HEADER = "[Tilemap]";
   static constexpr const char* ENTITIES_HEADER = "[Entities]";
   static constexpr const char* META_HEADER = "[Meta]";
 public:
  struct Background {
    std::filesystem::path texturePath;
    bool repeated;
    bool moving;
    sf::Vector2f size;
  };

  std::optional<Background> background;

  struct Tilemap {
    float tileSize;
    bool noTileset;
    std::filesystem::path tilesetPath;
    unsigned tilesetTileSize;
    std::size_t width;
    std::size_t height;
    std::vector<short> tiles;
    std::vector<bool> colliders;
  } tilemap;

  std::vector<EntityData> entities;

  struct Meta {
    bool followPlayer = true;
  } meta;

  static LevelData load(std::istream& in);
  void save(std::ostream& out);

 private:
  void loadBackgroundData(std::istream& in);
  void saveBackgroundData(std::ostream& out);

  void loadTilemapData(std::istream& in);
  void saveTilemapData(std::ostream& out);

  void loadEntityData(std::istream& in);
  void saveEntityData(std::ostream& out);

  void loadMetaData(std::istream& in);
  void saveMetaData(std::ostream& out);
};
