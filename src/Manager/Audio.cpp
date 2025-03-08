#include "Audio.h"

#include <SFML/Audio/Sound.hpp>

#include "Core/Logger.h"

// TODO(des): reduce copypasting

AudioManager::sounds_t::iterator AudioManager::loadSound(
    const sf::SoundBuffer& buffer) {
  return loaded_sounds.emplace(loaded_sounds.end(), buffer);
}

AudioManager::sounds_t::iterator AudioManager::loadSound(
    const sf::Sound& copy) {
  return loaded_sounds.emplace(loaded_sounds.end(), copy);
}

AudioManager::sounds_t::iterator AudioManager::playSound(
    const sf::SoundBuffer& buffer) {
  if (playing_sounds.size() > MAX_SOUNDS) {
    return playing_sounds.end();
  }
  auto it = playing_sounds.emplace(playing_sounds.end(), buffer);
  it->play();
  return it;
}

AudioManager::sounds_t::iterator AudioManager::playSound(
    const sf::Sound& copy) {
  if (playing_sounds.size() > MAX_SOUNDS) {
    return playing_sounds.end();
  }
  auto it = playing_sounds.emplace(playing_sounds.end(), copy);
  it->play();
  return it;
}

AudioManager::sounds_t::iterator AudioManager::playSound(
    sounds_t::iterator it) {
  if (playing_sounds.size() > MAX_SOUNDS) {
    return playing_sounds.end();
  }
  it->play();
  loaded_sounds.splice(playing_sounds.end(), playing_sounds, it);
  return std::prev(playing_sounds.end());
}

void AudioManager::clearStoppedSounds() {
  for (auto it = playing_sounds.begin(); it != playing_sounds.end();) {
    auto& sound = *it;
    if (sound.getStatus() == sf::Sound::Status::Stopped) {
      it = playing_sounds.erase(it);
    } else {
      ++it;
    }
  }
}

int AudioManager::playingSounds() { return playing_sounds.size(); }

void AudioManager::clear() {
  for (auto& sound : playing_sounds) {
    sound.stop();
  }
  loaded_sounds.clear();
  playing_sounds.clear();
}
