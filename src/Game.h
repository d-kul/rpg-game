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
#include "UI/Frame.h"
#include "UI/Text.h"

class Game;

#include "State.h"

class Game {
 private:
  // Initialization
  void initWindowConfig();
  void initKeybinds();
  void initManagers();
  void initState();

 public:
  void setFullscreen(bool fullscreen);
  void updateSize();
  void createWindow();

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
  static const keybinds_t default_keybinds;

  // Members
  std::string title;
  sf::ContextSettings contextSettings;
  sf::Clock clock;
  State* state = nullptr;
  Frame* loading_frame;
  Text* loading_text;

 public:
  sf::RenderWindow window;
  sf::VideoMode videoMode;
  bool fullscreen = false;

  EventManager eventManager;
  ResourceManager resourceManager;
  AudioManager audioManager;
  InteractibleManager interactibleManager;
  ColliderManager colliderManager;
  UIManager uiManager{window};

  keybinds_t keybinds;

 public:
  // Static members
  static const key_index_t key_index;
  static const key_storage_t key_storage;
};
