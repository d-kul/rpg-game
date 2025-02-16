#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "State.h"
#include "utility.h"

class Game {
 private:
  // Initialization
  void initWindow();
  void initKeybinds();
  void initRegistry();
  void initState();

 public:
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
  // Members
  bool fullscreen = false;
  sf::ContextSettings context_settings;
  sf::RenderWindow window;

  State* state = nullptr;
  sf::Clock clock;

  keybinds_t keybinds;
  static const keybinds_t default_keybinds;

 public:
  // Static members
  static const key_index_t key_index;
  static const key_storage_t key_storage;
};
