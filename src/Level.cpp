#include "Level.h"

#include <filesystem>
#include <ios>
#include <limits>
#include <memory>
#include <stdexcept>

#include "Core/Logger.h"
#include "Entity/Player.h"
#include "Entity/TileMapCollider.h"
#include "Game.h"

Level::Level() : resourceManager(Game::getResourceManager()) {}

void Level::loadFromFile(const std::filesystem::path& filename) {
  unload();
  DEBUG("loading level data from ", filename);
  std::ifstream in(filename);
  try {
    LevelData data = LevelData::load(in);
    Entity* player = nullptr;
    sf::Vector2u playerPos;
    loadBackground(data.background);
    loadTilemap(data.tilemap);
    loadEntities(data.entities, player, playerPos);
    if (player) {
      static_cast<Player*>(player)->setPosition(
          {data.tilemap.tileSize * playerPos.x,
           data.tilemap.tileSize * playerPos.y});
    }
  } catch (const std::filesystem::filesystem_error& e) {
    ERROR("failed to load level data from ", filename, ": ", e.what());
    unload();
    throw;
  } catch (const std::runtime_error& e) {
    ERROR("failed to load level data from ", filename, ": ", e.what());
    unload();
    throw;
  }
}

void Level::unload() {
  entities.clear();
  colliders.clear();
  resources.clear();
  background.unsetTexture();
}

void Level::update(sf::Time dt) {
  for (auto& entity : entities) {
    entity->update(dt);
  }
}

void Level::render(sf::RenderWindow& window) {
  background.setView(window.getView());
  window.draw(background);
  window.draw(tilemap);
  for (auto& entity : entities) {
    window.draw(*entity);
  }
}

void Level::skipLine(std::ifstream& in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), in.widen('\n'));
}

void Level::loadBackground(std::optional<LevelData::BackgroundData>& data) {
  if (!data) return;
  auto texture = resourceManager.load<sf::Texture>(data->texturePath);
  background.setTexture(texture.get(), data->size);
  texture->setRepeated(data->repeated);
  background.setMoving(data->moving);
  resources.push_back(texture);
}

void Level::loadTilemap(LevelData::TilemapData& data) {
  if (!data.noTileset) {
    auto tileset =
        resourceManager.load<TileSet>(data.tilesetPath, data.tilesetTileSize);
    tilemap.load(tileset.get(), std::move(data.tiles), data.tileSize,
                 sf::Vector2u(data.width, data.height));
    resources.push_back(tileset);
  }
  auto tilemap_collider = std::make_unique<TileMapCollider>();
  tilemap_collider->load(std::move(data.colliders), data.tileSize,
                         sf::Vector2u(data.width, data.height));
  colliders.push_back(std::move(tilemap_collider));
}

void Level::loadEntities(std::vector<LevelData::EntityData>& data,
                         Entity*& player, sf::Vector2u& playerPos) {
  for (auto& entity : data) {
    std::visit(overloaded{[&](LevelData::EntityData::PlayerData data) {
                            auto temp_player = std::make_unique<Player>();
                            player = temp_player.get();
                            playerPos = data.position;
                            entities.push_front(std::move(temp_player));
                          },
                          [&](LevelData::EntityData::InteractibleData data) {
                            // TODO: ...
                          }},
               entity.data);
  }
}
