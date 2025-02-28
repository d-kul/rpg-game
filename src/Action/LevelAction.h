#pragma once

#include "Action.h"
#include "State/GameState.h"

class LevelAction : public Action<LevelAction> {
 public:
  LevelAction(GameState& state, std::filesystem::path filename)
      : state(state), filename(std::move(filename)) {}

  bool update(sf::Time dt) override {
    state.loadNextLevel(filename);
    return false;
  }

 private:
  GameState& state;
  std::filesystem::path filename;
};
