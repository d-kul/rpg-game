#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Core/utility.h"
#include "Manager/Audio.h"
#include "Manager/Event.h"
#include "Manager/Interactible.h"
#include "Manager/Resource.h"
#include "State.h"

class Game {
 public:
  // Singleton interface
  static Game& getInstance();

  static sf::RenderWindow& getWindow();
  static sf::Vector2u& getWindowSize();
  static keybinds_t& getKeybinds();

  static EventManager& getEventManager();
  static ResourceManager& getResourceManager();
  static AudioManager& getAudioManager();
  static InteractibleManager& getInteractibleManager();

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
  sf::Vector2u windowSize;
  ResourceManager resourceManager;
  AudioManager audioManager;
  InteractibleManager interactibleManager;

  State* state = nullptr;
  sf::Clock clock;
  EventManager eventManager;
  keybinds_t keybinds;

 public:
  // Static members
  static const key_index_t key_index;
  static const key_storage_t key_storage;
};
