#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "../Action.h"
#include "Core/Logger.h"
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
    sound = &*manager.playSound(buffer);
    sound->setLooping(looping);
    sound->setPlayingOffset(sf::seconds(offsetSeconds));
    sound->setVolume(volume);
    DEBUG(sound->getStatus() == sf::Sound::Status::Playing);
  }

  bool update(sf::Time dt) override {
    return sound->getStatus() != sf::Sound::Status::Playing;
  }

  void end() override {
    sound = nullptr;
  }

 private:
  AudioManager& manager;
  sf::SoundBuffer& buffer;
  bool looping;
  float offsetSeconds;
  float volume;
  sf::Sound* sound = nullptr;
};
