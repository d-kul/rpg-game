#pragma once

#include <filesystem>
#include <optional>

#include "Action.h"

class LevelAction : public Action {
 public:
  LevelAction(std::optional<std::filesystem::path>& target,
              std::filesystem::path filename, int& playerSpot, int spot)
      : target(target),
        playerSpot(playerSpot),
        filename(filename),
        spot(spot) {}

  void start() override {
    playerSpot = spot;
    target = filename;
  }

 private:
  std::optional<std::filesystem::path>& target;
  std::filesystem::path filename;
  int& playerSpot;
  int spot;
};
