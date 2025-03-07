#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <list>
#include <optional>
#include <set>

#include "Action/ActionAction.h"
#include "Common/Data/Level.h"
#include "Entity/Background.h"
#include "Entity/Collider.h"
#include "Entity/Player.h"
#include "Entity/TileMap.h"
#include "Game.h"

class Level {
 public:
  static constexpr auto INTERACT_INTERVAL = sf::seconds(0.15f);

 public:
  Level(Game& game);
  ~Level();

  void loadFromFile(const std::filesystem::path& filename);
  void unload();

  void update(sf::Time dt);
  void render(sf::RenderWindow& window);

 private:
  void updateInput(sf::Time dt);

  void loadBackground(std::optional<LevelData::Background>& data);
  void loadTilemap(LevelData::Tilemap& data);
  void loadActions(
      std::vector<Action*>& actionRefs,
      std::vector<std::pair<ActionAction*, ActionData::Action>>& actionActions,
      LevelData& data);
  void loadEntities(std::vector<Action*>& actionRefs,
                    std::vector<InteractibleEntity*>& interactibleRefs,
                    LevelData::Tilemap& tilemap, std::vector<EntityData>& data);
  void bindRefs(
      std::vector<std::pair<ActionAction*, ActionData::Action>>& actionActions,
      std::vector<InteractibleEntity*>& interactibleRefs);

  void setImageFrameActive(bool active);
  void setTextFrameActive(bool active);
  void setMenuFrameActive(bool active);

  void onKeyPressed(sf::Event::KeyPressed keyPressed);
  void onKeyReleased(sf::Event::KeyReleased keyReleased);

 public:
  Player* player;

 private:
  Game& game;
  std::set<std::string> resourceNames;
  std::set<std::string> persistentResources;
  Background background;
  TileMap tilemap;
  std::shared_ptr<sf::Music> music;
  std::list<std::unique_ptr<Entity>> entities;
  std::list<std::unique_ptr<Collider>> colliders;
  bool followPlayer = true;
  Action* activeAction = nullptr;

  UIElement* menuFrame;

  UIElement* textFrame;
  sf::Text* textFrameText;
  bool textClosed;

  UIElement* imageFrame;
  sf::RectangleShape* imageFrameShape;
  bool imageClosed;

  std::vector<std::unique_ptr<Action>> actions;

  std::optional<std::filesystem::path> nextLevel;
  int nextLevelPlayerSpot = 0;

  bool menuPressed = false;
  sf::Time interactInterval = sf::Time::Zero;
  ConnectionGuard onKeyPressed_cg, onKeyReleased_cg;

  friend class GameState;
  friend class TextAction;
  friend class ImageAction;
  friend class MusicAction;
};
