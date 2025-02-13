#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <entt/entt.hpp>
#include <list>
#include <memory>

#include "ResourceLoader.h"
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

  template <typename Resource, typename... Args>
  static entt::resource<Resource> load(entt::id_type id, Args&&... args) {
    return instance_.getCache<Resource>()
        .load(id, std::forward<Args>(args)...)
        .first->second;
  }

  template <typename Resource, typename... Args>
  static std::pair<entt::resource<Resource>, retained_t::iterator> hold(
      entt::id_type id, Args&&... args) {
    auto handle = load<Resource>(id, std::forward<Args>(args)...);
    return {handle, instance_.retained.insert(instance_.retained.end(),
                                              handle.handle())};
  }

  template <typename Resource, typename... Args>
  static entt::resource<Resource> retain(entt::id_type id, Args&&... args) {
    auto [it, loaded] =
        instance_.getCache<Resource>().load(id, std::forward<Args>(args)...);
    if (loaded) instance_.retained.push_back(it->second.handle());
    return it->second;
  }

  static void release(retained_t::iterator it);

 private:
  // Initialization
  void initWindow();
  void initKeybinds();
  void initRegistry();
  void initState();

  // Constructors, destructor
  Game();
  ~Game();

  template <typename Resource>
  ResourceCache<Resource>& getCache() {
    return entt::any_cast<ResourceCache<Resource>&>(
        caches
            .try_emplace(entt::type_hash<Resource>::value(),
                         std::in_place_type<ResourceCache<Resource>>)
            .first->second);
  }

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

  // Resource management
  entt::dense_map<entt::id_type, entt::any> caches;
  retained_t retained;

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
