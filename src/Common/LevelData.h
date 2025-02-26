#pragma once

#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <iostream>
#include <optional>
#include <variant>
#include <vector>

class LevelData {
 public:
  struct BackgroundData {
    std::filesystem::path texturePath;
    bool repeated;
    bool moving;
    sf::Vector2f size;
  };

  std::optional<BackgroundData> background;

  struct TilemapData {
    float tileSize;
    bool noTileset;
    std::filesystem::path tilesetPath;
    unsigned tilesetTileSize;
    std::size_t width;
    std::size_t height;
    std::vector<short> tiles;
    std::vector<bool> colliders;
  } tilemap;

  struct EntityData {
    struct PlayerData {
      sf::Vector2u position;
    };
    struct InteractibleData {
      sf::Vector2u position;
    };
    std::variant<PlayerData, InteractibleData> data;
  };
  std::vector<EntityData> entities;

  static LevelData load(std::istream& in);
  void save(std::ostream& out);

 private:
  void loadBackgroundData(std::istream& in);
  void saveBackgroundData(std::ostream& out);

  void loadTilemapData(std::istream& in);
  void saveTilemapData(std::ostream& out);

  void saveEntityData(std::ostream& out);
  void loadEntityData(std::istream& in);
};
