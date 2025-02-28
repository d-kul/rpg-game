#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Core/utility.h"
#include "Manager/Audio.h"
#include "Manager/Collider.h"
#include "Manager/Event.h"
#include "Manager/Interactible.h"
#include "Manager/Resource.h"
#include "Manager/UI.h"
#include "State.h"

class Game {
 public:
  // Singleton interface
  static Game& getInstance();

  static sf::RenderWindow& getWindow();
  static sf::Vector2u& getWindowSize();
  static keybinds_t& getKeybinds();
  static State* getState();

  // TODO(des): manage the growing pile of managers
  static EventManager& getEventManager();
  static ResourceManager& getResourceManager();
  static AudioManager& getAudioManager();
  static InteractibleManager& getInteractibleManager();
  static ColliderManager& getColliderManager();
  static UIManager& getUIManager();

 private:
  // Initialization
  void initWindow();
  void initKeybinds();
  void initManagers();
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

  // TODO(des): manage the growing pile of managers
  EventManager eventManager;
  ResourceManager resourceManager;
  AudioManager audioManager;
  InteractibleManager interactibleManager;
  ColliderManager colliderManager;
  UIManager uiManager;

  State* state = nullptr;
  sf::Clock clock;
  keybinds_t keybinds;

 public:
  // Static members
  static const key_index_t key_index;
  static const key_storage_t key_storage;
};
