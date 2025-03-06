#include "Player.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

#include "Game.h"
#include "Resource/TileSet.h"

Player::Player(Game& game, float tileSize, float movementSpeed)
    : Player(game,
             *game.resourceManager
                  .retain<sf::Texture>(  // NOTE(des): a bit hacky but whatever
                      "resources/images/tilesets/omori.png"),
             tileSize, movementSpeed) {}

Player::Player(Game& game, const sf::Texture& texture, float tileSize,
               float movementSpeed)
    : keybinds(game.keybinds),
      interactibleManager(game.interactibleManager),
      colliderManager(game.colliderManager),
      Actor(texture, std::make_unique<TileSet>(texture, 32)),
      tileSize(tileSize),
      movementSpeed(movementSpeed) {
  sprite.setScale({tileSize / 32.f, tileSize / 32.f});
  updateAnimation();
}

void Player::update(sf::Time dt) {
  if (control) {
    updateInput();
    MovableEntity::update(dt);
    ensureAnimationState();
    DrawableEntity::update(dt);
  }
}

Action* Player::updateInteraction() {
  if (!positionSnapped || !interactKeyClick) return nullptr;
  sf::Vector2f position = getPosition();
  switch (animationState.direction) {
    case AnimationState::Up:
      position.y -= tileSize;
      break;
    case AnimationState::Down:
      position.y += tileSize;
      break;
    case AnimationState::Left:
      position.x -= tileSize;
      break;
    case AnimationState::Right:
      position.x += tileSize;
      break;
  }
  return interactibleManager.interact(position);
}

void Player::updateInput() {
  sf::Vector2f newInput;
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_UP"])) {
    newInput.y -= 1.f;
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_DOWN"])) {
    newInput.y += 1.f;
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_LEFT"])) {
    newInput.x -= 1.f;
  }
  if (sf::Keyboard::isKeyPressed(keybinds["MOVE_RIGHT"])) {
    newInput.x += 1.f;
  }
  if (newInput.y != 0 && newInput.x != 0) {
    if (input.y != 0) {
      newInput.x = 0;
    } else {
      newInput.y = 0;
    }
  }
  input = newInput;

  interactKeyClick = false;
  if (sf::Keyboard::isKeyPressed(keybinds["INTERACT"])) {
    if (!interactKeyDown) {
      interactKeyClick = true;
    }
    interactKeyDown = true;
  } else {
    interactKeyDown = false;
  }

  bool running = sf::Keyboard::isKeyPressed(keybinds["RUN"]);
  speed = running ? movementSpeed * 1.5f : movementSpeed;
  animationState.running = running;

  updateNextDestination();
}

void Player::updateNextDestination() {
  if (input.x != 0 || input.y != 0) {
    nextDestination = destination + input * tileSize;
  } else {
    nextDestination.reset();
  }
}

void Player::onSetDestination() {
  auto path_left = destination - getPosition();
  animationState.idle = path_left.x == 0 && path_left.y == 0;
  if (input.x != 0) {
    animationState.direction =
        input.x < 0 ? AnimationState::Left : AnimationState::Right;
  } else if (input.y != 0) {
    animationState.direction =
        input.y < 0 ? AnimationState::Up : AnimationState::Down;
  }
  if (colliderManager.checkCollision(destination)) {
    destination = getPosition();
    animationState.idle = true;
  }
}

void Player::ensureAnimationState() {
  if (prevAnimationState.direction == animationState.direction &&
      prevAnimationState.idle == animationState.idle &&
      (animationState.idle ||
       prevAnimationState.running == animationState.running))
    return;
  prevAnimationState = animationState;
  updateAnimation();
}

void Player::updateAnimation() {
  if (animationState.idle) {
    setFrames(idleAnimations[animationState.direction]);
  } else if (!animationState.running) {
    auto& frames = walkingAnimations[animationState.direction];
    setFrames(frames, 1);
    setFrameRate(frames.size());
  } else {
    auto& frames = runningAnimations[animationState.direction];
    setFrames(frames, 3);
    setFrameRate(frames.size() * RUNNING_FACTOR);
  }
}

void Player::onStop() { animationState.idle = true; }

const std::vector<int> Player::idleAnimations[4] = {
    {1 + 0 * 8},  // Down
    {1 + 1 * 8},  // Left
    {1 + 2 * 8},  // Right
    {1 + 3 * 8}   // Up
};

const std::vector<int> Player::walkingAnimations[4] = {
    {1 + 0 * 8, 2 + 0 * 8, 1 + 0 * 8, 0 + 0 * 8},  // Down
    {1 + 1 * 8, 2 + 1 * 8, 1 + 1 * 8, 0 + 1 * 8},  // Left
    {1 + 2 * 8, 2 + 2 * 8, 1 + 2 * 8, 0 + 2 * 8},  // Right
    {1 + 3 * 8, 2 + 3 * 8, 1 + 3 * 8, 0 + 3 * 8}   // Up
};

const std::vector<int> Player::runningAnimations[4] = {
    {0 + 4 * 8, 1 + 4 * 8, 2 + 4 * 8, 3 + 4 * 8, 4 + 4 * 8, 5 + 4 * 8,
     6 + 4 * 8, 7 + 4 * 8},  // Down
    {0 + 5 * 8, 1 + 5 * 8, 2 + 5 * 8, 3 + 5 * 8, 4 + 5 * 8, 5 + 5 * 8,
     6 + 5 * 8, 7 + 5 * 8},  // Left
    {0 + 6 * 8, 1 + 6 * 8, 2 + 6 * 8, 3 + 6 * 8, 4 + 6 * 8, 5 + 6 * 8,
     6 + 6 * 8, 7 + 6 * 8},  // Right
    {0 + 7 * 8, 1 + 7 * 8, 2 + 7 * 8, 3 + 7 * 8, 4 + 7 * 8, 5 + 7 * 8,
     6 + 7 * 8, 7 + 7 * 8}  // Up
};
