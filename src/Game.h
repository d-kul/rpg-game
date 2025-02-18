#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "AudioManager.h"
#include "Core/utility.h"
#include "EventHandler.h"
#include "ResourceManager.h"
#include "State.h"

class Game {
 public:
  // Singleton interface
  static Game& getInstance();
  static sf::RenderWindow& getWindow();
  static EventHandler& getEventHandler();
  static keybinds_t& getKeybinds();
  static ResourceManager& getResourceManager();
  static AudioManager& getAudioManager();

 private:
  // Initialization
  void initWindow();
  void initKeybinds();
  void initRegistry();
  void initState();

  // Constructor, destructor
  Game();
  ~Game();

 public:
  // Functionality
  void handleEvents();
  void update(sf::Time dt);
  void render();
  void run();

 private:
  static Game instance;
  static const keybinds_t default_keybinds;

  // Members
  bool fullscreen = false;
  sf::ContextSettings contextSettings;
  sf::RenderWindow window;
  ResourceManager resourceManager;
  AudioManager audioManager;

  State* state = nullptr;
  sf::Clock clock;
  EventHandler eventHandler;
  keybinds_t keybinds;

 public:
  // Static members
  static const key_index_t key_index;
  static const key_storage_t key_storage;
};
