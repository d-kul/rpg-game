#include "Level.h"

#include <SFML/Window/Keyboard.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "Action/ActionAction.h"
#include "Action/ImageAction.h"
#include "Action/LevelAction.h"
#include "Action/MusicAction.h"
#include "Action/SoundAction.h"
#include "Action/TextAction.h"
#include "Core/Logger.h"
#include "Entity/Interactible.h"
#include "Entity/Player.h"
#include "Entity/TileMapCollider.h"
#include "Entity/Trigger.h"
#include "Game.h"

Level::Level(Game& game) : game(game) {}

Level::~Level() {
  for (auto& name : resourceNames) {
    game.resourceManager.release(name);
  }
  for (auto& name : persistentResources) {
    game.resourceManager.release(name);
  }
}

void Level::loadFromFile(const std::filesystem::path& filename) {
  unload();
  DEBUG("loading level data from ", filename);
  std::ifstream in(filename);
  std::set<std::string> oldResourceNames = std::move(resourceNames);
  try {
    LevelData data = LevelData::load(in);
    loadBackground(data.background);
    loadTilemap(data.tilemap);
    std::vector<Action*> actionRefs;
    std::vector<std::pair<ActionAction*, ActionData::Action>> actionActions;
    std::vector<InteractibleEntity*> interactibleRefs;
    loadActions(actionRefs, actionActions, data);
    loadEntities(actionRefs, interactibleRefs, data.tilemap, data.entities);
    bindRefs(actionActions, interactibleRefs);
    for (auto& name : oldResourceNames) {
      if (!resourceNames.count(name)) {
        game.resourceManager.release(name);
      }
    }
    oldResourceNames.clear();
    followPlayer = data.meta.followPlayer;
    if (data.meta.startAction != -1) {
      activeAction = actionRefs.at(data.meta.startAction);
      activeAction->start();
    }
    nextLevelPlayerSpot = 0;
    onKeyPressed_cg = game.eventManager.bind<sf::Event::KeyPressed>(
        &Level::onKeyPressed, this);
    onKeyReleased_cg = game.eventManager.bind<sf::Event::KeyReleased>(
        &Level::onKeyReleased, this);
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
  actions.clear();
  tilemap.clear();
  background.unsetTexture();
  player = nullptr;
  if (activeAction) {
    activeAction->end();
  }
  activeAction = nullptr;
  onKeyPressed_cg.disconnect();
  onKeyReleased_cg.disconnect();
}

void Level::update(sf::Time dt) {
  if (interactInterval > sf::Time::Zero) interactInterval -= dt;
  if (menuFrame->isActive()) return;
  for (auto& entity : entities) {
    if (entity.get() != player) {
      entity->update(dt);
      continue;
    }
    player->update(dt);
    if (!activeAction && interactInterval <= sf::Time::Zero) {
      activeAction = player->updateInteraction();
      if (activeAction) {
        interactInterval = INTERACT_INTERVAL;
        activeAction->start();
      }
    }
  }
  Action::update(activeAction, dt);
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

void Level::updateInput(sf::Time dt) {}

void Level::loadBackground(std::optional<LevelData::Background>& data) {
  if (!data) return;
  auto texture = game.resourceManager.retain<sf::Texture>(data->texturePath);
  background.setTexture(texture.get(), data->size);
  texture->setRepeated(data->repeated);
  background.setMoving(data->moving);
  resourceNames.insert(data->texturePath);
}

void Level::loadTilemap(LevelData::Tilemap& data) {
  if (!data.noTileset) {
    auto tilesetTexture =
        game.resourceManager.retain<sf::Texture>(data.tilesetPath);
    TileSet tileset{*tilesetTexture, data.tilesetTileSize};
    tilemap.load(tileset, std::move(data.tiles), data.tileSize,
                 sf::Vector2u(data.width, data.height));
    resourceNames.insert(data.tilesetPath);
  }
  auto tilemap_collider =
      std::make_unique<TileMapCollider>(game.colliderManager);
  tilemap_collider->load(std::move(data.colliders), data.tileSize,
                         sf::Vector2u(data.width, data.height));
  colliders.push_back(std::move(tilemap_collider));
}

void Level::loadActions(
    std::vector<Action*>& actionRefs,
    std::vector<std::pair<ActionAction*, ActionData::Action>>& actionActions,
    LevelData& data) {
  for (auto& action : data.actions) {
    std::visit(
        overloaded{
            [&](ActionData::Text& text) {
              actions.emplace_back(
                  std::make_unique<TextAction>(*this, std::move(text.text)));
              actionRefs.push_back(actions.back().get());
            },
            [&](ActionData::Image& image) {
              auto texture =
                  game.resourceManager.retain<sf::Texture>(image.filename);
              actions.emplace_back(
                  std::make_unique<ImageAction>(*this, *texture));
              persistentResources.insert(image.filename);
              actionRefs.push_back(actions.back().get());
            },
            [&](ActionData::Sound& sound) {
              auto soundBuffer =
                  game.resourceManager.retain<sf::SoundBuffer>(sound.filename);
              actions.emplace_back(std::make_unique<SoundAction>(
                  game.audioManager, *soundBuffer, sound.looping,
                  sound.offsetSeconds, sound.volume));
              persistentResources.insert(sound.filename);
              actionRefs.push_back(actions.back().get());
            },
            [&](ActionData::Music& music) {
              actions.emplace_back(std::make_unique<MusicAction>(
                  *this, music.filename, music.looping, music.offsetSeconds,
                  music.volume));
              persistentResources.insert(music.filename);
              actionRefs.push_back(actions.back().get());
            },
            [&](ActionData::Level& level) {
              actions.emplace_back(std::make_unique<LevelAction>(
                  nextLevel, level.filename, nextLevelPlayerSpot,
                  level.playerSpot));
              actionRefs.push_back(actions.back().get());
            },
            [&](ActionData::Action& action) {
              auto actionAction =
                  std::make_unique<ActionAction>(nullptr, nullptr);
              actionActions.push_back({actionAction.get(), action});
              actions.emplace_back(std::move(actionAction));
              actionRefs.push_back(actions.back().get());
            },
            [&](auto&) {
              // TODO(des): other actions
            }},
        action.data);
  }
  for (auto& [actionAction, action] : actionActions) {
    actionAction->action =
        action.action == -1 ? nullptr : actionRefs.at(action.action);
  }
  for (std::size_t i = 0; i < actions.size(); i++) {
    int next = data.nextAction.at(i);
    actions[i]->setNext(next == -1 ? nullptr : actionRefs.at(next));
  }
}

void Level::loadEntities(std::vector<Action*>& actionRefs,
                         std::vector<InteractibleEntity*>& interactibleRefs,
                         LevelData::Tilemap& tilemap,
                         std::vector<EntityData>& data) {
  int players = 0;
  for (auto& entity : data) {
    InteractibleEntity* interactible = nullptr;
    std::visit(
        overloaded{
            [&](EntityData::Player data) {
              if (players++ != nextLevelPlayerSpot) return;
              auto player = std::make_unique<Player>(game, tilemap.tileSize);
              player->setPosition(sf::Vector2f{entity.position} *
                                  tilemap.tileSize);
              player->setDestination(sf::Vector2f{entity.position} *
                                     tilemap.tileSize);
              this->player = player.get();
              entities.push_front(std::move(player));
            },
            [&](EntityData::Character& data) {
              // TODO(des): sprite data
              auto character = std::make_unique<InteractibleEntity>(
                  game.interactibleManager,
                  data.action == -1 ? nullptr : actionRefs.at(data.action));
              character->setPosition(sf::Vector2f{entity.position} *
                                     tilemap.tileSize);
              interactible = character.get();
              entities.push_back(std::move(character));
            },
            [&](EntityData::Prop& data) {
              // TODO(des): sprite data
              auto prop = std::make_unique<InteractibleEntity>(
                  game.interactibleManager,
                  data.action == -1 ? nullptr : actionRefs.at(data.action));
              prop->setPosition(sf::Vector2f{entity.position} *
                                tilemap.tileSize);
              interactible = prop.get();
              entities.push_back(std::move(prop));
            },
            [&](EntityData::Trigger& data) {
              auto trigger = std::make_unique<Trigger>(
                  game.interactibleManager,
                  data.action == -1 ? nullptr : actionRefs.at(data.action));
              trigger->setPosition(sf::Vector2f{entity.position} *
                                   tilemap.tileSize);
              interactible = static_cast<InteractibleEntity*>(trigger.get());
              entities.push_back(std::move(trigger));
            }},
        entity.data);
    interactibleRefs.push_back(interactible);
  }
}

void Level::bindRefs(
    std::vector<std::pair<ActionAction*, ActionData::Action>>& actionActions,
    std::vector<InteractibleEntity*>& interactibleRefs) {
  for (auto& [actionAction, action] : actionActions) {
    actionAction->holder =
        action.holder == -1 ? nullptr : interactibleRefs.at(action.holder);
  }
}

void Level::setImageFrameActive(bool active) {
  if (active == imageFrame->isActive()) return;
  imageFrame->setActive(active);
  if (player) {
    if (active) {
      player->control = false;
    } else if (!textFrame->isActive()) {
      player->control = true;
    }
  }
}

void Level::setTextFrameActive(bool active) {
  if (active == textFrame->isActive()) return;
  textFrame->setActive(active);
  if (player) {
    if (active) {
      player->control = false;
    } else if (!imageFrame->isActive()) {
      player->control = true;
    }
  }
}

void Level::setMenuFrameActive(bool active) {
  if (active == menuFrame->isActive()) return;
  menuFrame->setActive(active);
  if (music) {
    if (active) {
      music->pause();
    } else {
      music->play();
    }
  }
}

void Level::onKeyPressed(sf::Event::KeyPressed keyPressed) {
  if (sf::Keyboard::isKeyPressed(game.keybinds["QUIT"]) && !menuPressed) {
    menuPressed = true;
    setMenuFrameActive(!menuFrame->isActive());
  }
  if (!menuFrame->isActive() &&
      sf::Keyboard::isKeyPressed(game.keybinds["INTERACT"]) &&
      interactInterval <= sf::Time::Zero) {
    if (textFrame->isActive()) {
      interactInterval = INTERACT_INTERVAL;
      textClosed = true;
    } else if (imageFrame->isActive()) {
      interactInterval = INTERACT_INTERVAL;
      imageClosed = true;
    }
  }
}

void Level::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == game.keybinds["QUIT"]) {
    menuPressed = false;
  }
}
