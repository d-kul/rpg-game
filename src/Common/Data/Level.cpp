#include "Level.h"

#include <stdexcept>

#include "Core/Logger.h"
#include "Core/utility.h"

static void error(std::string what_arg) {
  ERROR(what_arg);
  throw std::runtime_error{what_arg};
}

LevelData LevelData::load(std::istream& in) {
  if (!in) {
    error("the input stream is not open");
  }
  LevelData data;
  std::string line;
  bool tilemapLoaded = false;
  while (std::getline(in, line)) {
    if (line.empty() || line[0] != '[' || line.back() != ']') continue;
    if (line == BACKGROUND_HEADER) {
      data.loadBackgroundData(in);
    } else if (line == TILEMAP_HEADER) {
      data.loadTilemapData(in);
      tilemapLoaded = true;
    } else if (line == ENTITIES_HEADER) {
      if (!tilemapLoaded)
        error("tilemap data should be located before entity data");
      data.loadEntityData(in);
    } else if (line == META_HEADER) {
      data.loadMetaData(in);
    }
  }
  if (!tilemapLoaded) {
    error("no tilemap data in file");
  }
  return data;
}

void LevelData::save(std::ostream& out) {
  if (!out) {
    error("the output stream is not open");
  }
  saveBackgroundData(out);
  saveTilemapData(out);
  saveEntityData(out);
  saveMetaData(out);
}

void LevelData::loadBackgroundData(std::istream& in) {
  background.emplace();
  in >> background->texturePath >> background->repeated >> background->moving >>
      background->size.x >> background->size.y >> std::ws;
  DEBUG("texturePath:", background->texturePath.c_str(), "; ",
        "repeated,moving:", background->repeated, ",", background->moving, "; ",
        "size:", background->size.x, ",", background->size.y);
}

void LevelData::saveBackgroundData(std::ostream& out) {
  if (!background) return;
  out << BACKGROUND_HEADER << '\n'
      << background->texturePath << ' ' << background->repeated << ' '
      << background->moving << ' ' << background->size.x << ' '
      << background->size.y << '\n';
}

void LevelData::loadTilemapData(std::istream& in) {
  in >> tilemap.tileSize >> tilemap.noTileset >> std::ws;
  if (!tilemap.noTileset) {
    in >> tilemap.tilesetPath >> tilemap.tilesetTileSize;
  }
  in >> tilemap.width >> tilemap.height;
  if (!tilemap.noTileset) {
    tilemap.tiles.resize(tilemap.width * tilemap.height);
  }
  tilemap.colliders.resize(tilemap.width * tilemap.height);
  for (int i = 0; i < tilemap.height; i++) {
    for (int j = 0; j < tilemap.width; j++) {
      if (!tilemap.noTileset) {
        in >> tilemap.tiles[i * tilemap.width + j];
      }
      bool val;
      in >> val;
      tilemap.colliders[i * tilemap.width + j] = val;
    }
  }
  in >> std::ws;
  if (tilemap.noTileset) {
    DEBUG("tileSize:", tilemap.tileSize, "; ", "width,height:", tilemap.width,
          ",", tilemap.height);
  } else {
    DEBUG("tileset:", tilemap.tilesetPath.c_str(), "; ",
          "tilesetTileSize:", tilemap.tilesetTileSize, "; ",
          "tileSize:", tilemap.tileSize, "; ", "width,height:", tilemap.width,
          ",", tilemap.height);
  }
}

void LevelData::saveTilemapData(std::ostream& out) {
  out << TILEMAP_HEADER << '\n'
      << tilemap.tileSize << ' ' << tilemap.noTileset << '\n';
  if (!tilemap.noTileset) {
    out << tilemap.tilesetPath << ' ' << tilemap.tilesetTileSize << '\n';
  }
  out << tilemap.width << ' ' << tilemap.height << '\n';
  for (int i = 0; i < tilemap.height; i++) {
    for (int j = 0; j < tilemap.width; j++) {
      if (!tilemap.noTileset) {
        out << tilemap.tiles[i * tilemap.width + j] << ' ';
      }
      out << tilemap.colliders[i * tilemap.width + j] << ' ';
    }
    out << '\n';
  }
}

void LevelData::loadEntityData(std::istream& in) {
  std::size_t entityAmount;
  in >> entityAmount >> std::ws;
  DEBUG("entities:", entityAmount);
  for (std::size_t i = 0; i < entityAmount; i++) {
    entities.push_back(EntityData::load(in));
  }
}

void LevelData::saveEntityData(std::ostream& out) {
  out << ENTITIES_HEADER << '\n' << entities.size() << '\n';
  for (auto& entityData : entities) {
    entityData.save(out);
  }
}

void LevelData::loadMetaData(std::istream& in) {
  in >> meta.followPlayer >> std::ws;
}

void LevelData::saveMetaData(std::ostream& out) {
  out << META_HEADER << '\n' << meta.followPlayer << '\n';
}
