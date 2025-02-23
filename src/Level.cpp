#include "Level.h"

#include <ios>
#include <limits>
#include <memory>
#include <string>

#include "Core/Logger.h"
#include "Entity/TileMapCollider.h"
#include "Game.h"

Level::Level() : resourceManager(Game::getResourceManager()) {}

void Level::loadFromFile(const std::filesystem::path& filename) {
  unload();
  DEBUG("loading level from ", filename);
  std::ifstream in(filename);
  std::string header;

  bool tilemap_loaded = false;
  while (std::getline(in, header)) {
    DEBUG("header is ", header);
    if (header == "[Background]") {
      loadBackground(in);
    } else if (header == "[Tilemap]") {
      loadTilemap(in);
      tilemap_loaded = true;
    } else if (header == "[Player]") {
      loadPlayer(in);
    } else if (header == "[Entities]") {
      loadEntities(in);
    } else {
      break;
    }
  }
  if (!tilemap_loaded) {
    ERROR("no tilemap info in file ", filename);
    return;
  }
  if (player) {
    player->setPosition(player->getPosition() * tilemap.tileSize);
  }
}

void Level::unload() {
  player.reset();
  entities.clear();
  colliders.clear();
  resources.clear();
  background.unsetTexture();
}

void Level::update(sf::Time dt) {
  player->update(dt);
  for (auto& entity : entities) {
    entity->update(dt);
  }
}

void Level::render(sf::RenderWindow& window) {
  background.setView(window.getView());
  window.draw(background);
  window.draw(tilemap);
  window.draw(*player);
  for (auto& entity : entities) {
    window.draw(*entity);
  }
}

void Level::skipLine(std::ifstream& in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), in.widen('\n'));
}

void Level::loadBackground(std::ifstream& in) {
  std::string line;
  std::getline(in, line);
  auto texture = resourceManager.load<sf::Texture>(line);
  bool repeated, moving;
  sf::Vector2f size;
  in >> repeated >> moving >> size.x >> size.y;
  skipLine(in);
  background.setTexture(texture.get(), size, repeated);
  background.setMoving(moving);
  resources.push_back(texture);
  SDEBUG(" ", "got background", line, repeated, moving, size.x, size.y);
}

void Level::loadTilemap(std::ifstream& in) {
  int width, height;
  in >> tilemap.tileSize;
  skipLine(in);
  std::string line;
  std::getline(in, line);
  bool no_tileset = line == "<none>";
  std::shared_ptr<TileSet> tileset;
  if (!no_tileset) {
    int tileset_tile_size;
    in >> tileset_tile_size;
    tileset = resourceManager.load<TileSet>(line, tileset_tile_size);
  }
  in >> width >> height;
  std::vector<short> tiles(no_tileset ? 0 : (width * height));
  std::vector<bool> tile_colliders(width * height);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (!no_tileset) {
        in >> tiles[i * width + j];
      }
      bool val;
      in >> val;
      tile_colliders[i * width + j] = val;
    }
  }
  skipLine(in);
  if (!no_tileset) {
    tilemap.load(tileset.get(), std::move(tiles), tilemap.tileSize,
                 sf::Vector2u(width, height));
    resources.push_back(tileset);
  }
  auto tilemap_collider = std::make_unique<TileMapCollider>();
  tilemap_collider->load(std::move(tile_colliders), tilemap.tileSize,
                         sf::Vector2u(width, height));
  colliders.push_back(std::move(tilemap_collider));
  SDEBUG(" ", "got tilemap", line, width, height);
}

void Level::loadPlayer(std::ifstream& in) {
  player = std::make_unique<Player>();
  int pos_x, pos_y;
  in >> pos_x >> pos_y;
  skipLine(in);
  // NOTE: this is a big stinky hack to get rid of external state
  player->setPosition(sf::Vector2f(pos_x, pos_y));
  DEBUG("got player ", pos_x, ",", pos_y);
}

void Level::loadEntities(std::ifstream& in) {
  int amount;
  in >> amount;
  DEBUG("got ", amount, " entities");
  for (int i = 0; i < amount; i++) {
    // TODO: ??? (maybe include player here)
  }
}
