#include "Level.h"

#include <filesystem>
#include <fstream>
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
    loadEntities(data.tilemap, data.entities);
    followPlayer = data.meta.followPlayer;
    DEBUG("followPlayer:", followPlayer);
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
  player = nullptr;
  activeAction.reset();  // NOTE(des): what if the action needs to end?
}

void Level::update(sf::Time dt) {
  for (auto& entity : entities) {
    if (entity.get() != player) {
      entity->update(dt);
      continue;
    }
    player->update(dt);
    if (activeAction) {
      player->Actor::update(dt);
    } else {
      activeAction = player->updateInteraction();
    }
  }
  AbstractAction::update(activeAction, dt);
}

void Level::render(sf::RenderWindow& window) {
  if (followPlayer && player) {
    auto view = window.getView();
    view.setCenter(player->getPosition() +
                   player->getLocalBounds().getCenter());
    window.setView(view);
  }
  background.setView(window.getView());
  window.draw(background);
  window.draw(tilemap);
  for (auto& entity : entities) {
    window.draw(*entity);
  }
}

void Level::loadBackground(std::optional<LevelData::Background>& data) {
  if (!data) return;
  auto texture = resourceManager.load<sf::Texture>(data->texturePath);
  background.setTexture(texture.get(), data->size);
  texture->setRepeated(data->repeated);
  background.setMoving(data->moving);
  resources.push_back(texture);
}

void Level::loadTilemap(LevelData::Tilemap& data) {
  if (!data.noTileset) {
    auto tilesetTexture = resourceManager.load<sf::Texture>(data.tilesetPath);
    TileSet tileset{*tilesetTexture, data.tilesetTileSize};
    tilemap.load(tileset, std::move(data.tiles), data.tileSize,
                 sf::Vector2u(data.width, data.height));
    resources.push_back(tilesetTexture);
  }
  auto tilemap_collider = std::make_unique<TileMapCollider>();
  tilemap_collider->load(std::move(data.colliders), data.tileSize,
                         sf::Vector2u(data.width, data.height));
  colliders.push_back(std::move(tilemap_collider));
}

void Level::loadEntities(LevelData::Tilemap& tilemap,
                         std::vector<EntityData>& data) {
  for (auto& entity : data) {
    std::visit(
        overloaded{[&](EntityData::Player
                           data) {  // TODO(des): add support for movementSpeed
                     auto player = std::make_unique<Player>(tilemap.tileSize);
                     player->setPosition(sf::Vector2f{data.position} *
                                         tilemap.tileSize);
                     player->setDestination(sf::Vector2f{data.position} *
                                            tilemap.tileSize);
                     this->player = player.get();
                     entities.push_front(std::move(player));
                   },
                   [&](auto&& data) {
                     // TODO(des): ...
                   }},
        entity.data);
  }
}
