#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "../Action.h"
#include "Manager/Audio.h"

class SoundAction : public Action {
 public:
  SoundAction(AudioManager& manager, sf::SoundBuffer& buffer,
              bool looping = false, float offsetSeconds = 0.f,
              float volume = 100.f)
      : manager(manager),
        buffer(buffer),
        looping(looping),
        offsetSeconds(offsetSeconds),
        volume(volume) {}

  void start() override {
    auto& sound = *manager.playSound(buffer);
    sound.setLooping(looping);
    sound.setPlayingOffset(sf::seconds(offsetSeconds));
    sound.setVolume(volume);
  }

 private:
  AudioManager& manager;
  sf::SoundBuffer& buffer;
  bool looping;
  float offsetSeconds;
  float volume;
};
