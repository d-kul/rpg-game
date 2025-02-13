#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <entt/entt.hpp>
#include <memory>

#include "State.h"
#include "System/EventHandlerSystem.h"
#include "System/MovementSystem.h"
#include "System/PlayerSystem.h"
#include "System/RenderSystem.h"
#include "System/SoundSystem.h"
#include "utility.h"

class Game {
 public:
  // Singleton access
  static Game& instance();

  // Singleton member access
  static sf::RenderWindow& getWindow();
  static keybinds_t& getKeybinds();
  static entt::registry& getRegistry();

 private:
  // Initialization
  void initWindow();
  void initKeybinds();
  void initRegistry();
  void initState();

 private:
  // Constructors, destructor
  Game();
  ~Game();

 public:
  // Functionality
  void handleEvents();
  void update(sf::Time dt);
  void render();
  void run();

 private:
  // Singleton static member
  static Game instance_;

  // Members
  bool fullscreen = false;
  sf::ContextSettings context_settings;
  sf::Clock clock;
  std::unique_ptr<State> state;

  // Accessible through singleton
  sf::RenderWindow window;
  keybinds_t keybinds;
  entt::registry registry;

  // Systems
  EventHandlerSystem event_handler_system;
  RenderSystem render_system;
  SoundSystem sound_system;
  MovementSystem movement_system;
  PlayerSystem player_system;

  static const keybinds_t default_keybinds;

 public:
  // Static members
  static const key_index_t key_index;
  static const key_storage_t key_storage;
};
