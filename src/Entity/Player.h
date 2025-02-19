#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include "AnimatedSprite.h"
#include "AudioManager.h"
#include "Core/utility.h"
#include "Entity.h"

class Player : public Entity {
 public:
  static constexpr sf::Time EMIT_FREQ = sf::seconds(0.07);

 public:
  Player(float movementSpeed = 300.f, float tileSize = 64.f);

  void update(sf::Time dt) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  enum animationDirection_t { Down, Left, Right, Up };
  void setAnimationDirection(animationDirection_t direction, bool isRunning);
  void unsetAnimationIdle();
  void updateAnimation();

 private:
  keybinds_t& keybinds;
  AudioManager& audioManager;
  sf::Time elapsedTime = sf::Time::Zero;

  std::shared_ptr<sf::SoundBuffer> soundBuffer;
  std::shared_ptr<TileSet> spriteSheet;
  AnimatedSprite sprite;

  float movementSpeed;
  float tileSize;
  sf::Vector2f movementDestination;

  animationDirection_t animationDirection = Down;
  bool animationIdle = true;
  bool animationRunning = false;
};
