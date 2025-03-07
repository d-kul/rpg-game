#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include "Core/utility.h"
#include "Entity/Actor.h"
#include "Game.h"
#include "Manager/Collider.h"
#include "Manager/Interactible.h"

class Player : public Actor {
 private:
  static const std::vector<int> idleAnimations[4];
  static const std::vector<int> walkingAnimations[4];
  static const std::vector<int> runningAnimations[4];
  static constexpr float RUNNING_FACTOR = 1.5f;

 public:
  Player(Game& game, float tileSize = 64.f, float movementSpeed = 300.f);
  Player(Game& game, const sf::Texture& texture, float tileSize = 64.f,
         float movementSpeed = 300.f);

  void update(sf::Time dt) override;
  Action* updateInteraction();

 private:
  void updateInput();
  void updateNextDestination();
  void ensureAnimationState();
  void updateAnimation();

  void onSetDestination() override;
  void onSnap() override;
  void onStop() override;

 public:
  bool control = true;

 private:
  struct AnimationState {
    enum { Down, Left, Right, Up } direction = Down;
    bool idle = true;
    bool running = false;
  };

  keybinds_t& keybinds;
  InteractibleManager& interactibleManager;
  ColliderManager& colliderManager;

  AnimationState animationState, prevAnimationState;
  Action* triggerAction = nullptr;

  float tileSize;
  float movementSpeed;
  bool interactKeyDown = false;
  bool interactKeyClick = false;
  sf::Vector2f input;
};
