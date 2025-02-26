#include "LevelData.h"

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
    if (line == "[Background]") {
      data.loadBackgroundData(in);
    } else if (line == "[Tilemap]") {
      data.loadTilemapData(in);
      tilemapLoaded = true;
    } else if (line == "[Entities]") {
      data.loadEntityData(in);
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
}

static void skipLine(std::istream& in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void LevelData::loadBackgroundData(std::istream& in) {
  background.emplace();
  std::string line;
  std::getline(in, line);
  background->texturePath = line;
  in >> background->repeated >> background->moving >> background->size.x >>
      background->size.y;
  skipLine(in);
  DEBUG("texturePath:", background->texturePath.c_str(), "; ",
        "repeated,moving:", background->repeated, ",", background->moving, "; ",
        "size:", background->size.x, ",", background->size.y);
}

void LevelData::saveBackgroundData(std::ostream& out) {
  if (!background) return;
  out << "[Background]" << '\n';
  out << background->texturePath.c_str() << '\n';
  out << background->repeated << background->moving << background->size.x
      << background->size.y << '\n';
}

void LevelData::loadTilemapData(std::istream& in) {
  std::string line;
  in >> tilemap.tileSize;
  skipLine(in);
  std::getline(in, line);
  tilemap.noTileset = line == "<none>";
  if (!tilemap.noTileset) {
    tilemap.tilesetPath = line;
    in >> tilemap.tilesetTileSize;
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
  skipLine(in);
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
  out << "[Tilemap]" << '\n';
  out << tilemap.tileSize << '\n';
  if (!tilemap.noTileset) {
    out << tilemap.tilesetPath << '\n' << tilemap.tilesetTileSize << ' ';
    if (tilemap.tilesetTileSize < 1) {
      error("Invalid tile size: " + std::to_string(tilemap.tilesetTileSize));
    }
  } else {
    out << "<none>" << '\n';
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

void LevelData::saveEntityData(std::ostream& out) {
  out << entities.size() << '\n';
  auto visitor = overloaded{[&](EntityData::PlayerData playerData) {
                              out << "(Player)" << '\n';
                              out << playerData.position.x << ' '
                                  << playerData.position.y << '\n';
                            },
                            [&](EntityData::InteractibleData interactibleData) {
                              out << "(Interactible)" << '\n';
                              out << interactibleData.position.x << ' '
                                  << interactibleData.position.y << '\n';
                            }};
  for (auto& entityData : entities) {
    std::visit(visitor, entityData.data);
  }
}

void LevelData::loadEntityData(std::istream& in) {
  std::size_t entityAmount;
  in >> entityAmount;
  skipLine(in);
  DEBUG("entities:", entityAmount);

  std::string header;
  for (std::size_t i = 0; i < entityAmount; i++) {
    std::getline(in, header);
    if (header.empty() || header[0] != '(' || header.back() != ')') {
      error("No expected header in brackets while reading entity data");
    }
    if (header == "(Player)") {
      EntityData::PlayerData playerData;
      in >> playerData.position.x >> playerData.position.y;
      entities.push_back({std::move(playerData)});
      DEBUG("(player) position:", playerData.position.x, ",",
            playerData.position.y);
    } else if (header == "(Interactible)") {
      EntityData::InteractibleData interactibleData;
      in >> interactibleData.position.x >> interactibleData.position.y;
      entities.push_back({std::move(interactibleData)});
      DEBUG("(interactible) position:", interactibleData.position.x, ",",
            interactibleData.position.y);
    } else {
      error("Unexpected header " + header);
    }
  }
}
