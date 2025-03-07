#pragma once

#include "../Action.h"
#include "Level.h"

class MusicAction : public Action {
 public:
  MusicAction(Level& level, std::filesystem::path filename,
              bool looping = false, float offsetSeconds = 0.f,
              float volume = 100.f)
      : level(level),
        filename(filename),
        looping(looping),
        offsetSeconds(offsetSeconds),
        volume(volume) {}

  void start() override {
    auto music = level.game.resourceManager.retain<sf::Music>(filename);
    if (music == level.music) return;
    if (level.music) {
      level.music->stop();
    }
    music->setLooping(looping);
    music->setPlayingOffset(sf::seconds(offsetSeconds));
    music->setVolume(volume);
    level.music = music;
    level.music->play();
  }

 private:
  Level& level;
  std::filesystem::path filename;
  bool looping;
  float offsetSeconds;
  float volume;
};
