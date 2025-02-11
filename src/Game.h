#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

#include "State.h"

class Game {
 public:
  using states_t = std::stack<std::unique_ptr<State>>;
  using keybinds_t = std::unordered_map<std::string, sf::Keyboard::Key>;
  using key_index_t = std::unordered_map<std::string, sf::Keyboard::Key>;
  using key_storage_t = std::unordered_map<sf::Keyboard::Key, const char*>;

  // Singleton access
  static Game& instance();

  // Singleton member access
  static sf::RenderWindow& getWindow();
  static keybinds_t& getKeybinds();
  static states_t& getStates();

 private:
  // Initialization
  void initWindow();
  void initKeybinds();
  void initStates();

 private:
  // Constructors, destructor
  Game();
  virtual ~Game();

 public:
  // Functionality
  void pollEvents();
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

  // Accessible through singleton
  sf::RenderWindow window;
  keybinds_t keybinds;
  states_t states;

  static const keybinds_t default_keybinds;

 public:
  // Static members
  static const key_index_t key_index;
  static const key_storage_t key_storage;
};
