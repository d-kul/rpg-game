#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "../Action.h"
#include "Manager/Audio.h"

class SoundAction : public Action {
 public:
  SoundAction(AudioManager& manager, sf::SoundBuffer* buffer = nullptr)
      : manager(manager), buffer(buffer) {}
  void start() override {
    if (buffer) {
      manager.playSound(*buffer);
    }
  }

 private:
  AudioManager& manager;
  sf::SoundBuffer* buffer;
};
