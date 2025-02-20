#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include "AnimatedSprite.h"
#include "Core/utility.h"
#include "Entity.h"
#include "Manager/Audio.h"
#include "Manager/Collider.h"
#include "Manager/Interactible.h"

class Player : public Entity {
 public:
  static constexpr sf::Time EMIT_FREQ = sf::seconds(0.07);
  static constexpr float CONTROL_RANGE = 0.05f;

 public:
  Player(float tileSize = 64.f, float movementSpeed = 300.f);

  void setPosition(sf::Vector2f position);
  void setDestination(sf::Vector2f destination);

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  void updateInput();
  void updateMovement(sf::Time dt);
  void updateAnimationState();
  void updateAnimation();

 private:
  struct AnimationState {
    enum { Down, Left, Right, Up } direction = Down;
    bool idle = true;
    bool running = false;
  };

  keybinds_t& keybinds;
  sf::RenderWindow& window;
  AudioManager& audioManager;
  InteractibleManager& interactibleManager;
  ColliderManager& colliderManager;
  sf::Time elapsedTime = sf::Time::Zero;

  std::shared_ptr<TileSet> spriteSheet;
  AnimatedSprite sprite;
  AnimationState animationState;

  float tileSize;
  float movementSpeed;
  bool pressed = false;
  sf::Vector2f input, movementDestination;
};
