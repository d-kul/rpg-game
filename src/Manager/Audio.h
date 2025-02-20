#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <list>

class AudioManager {
 public:
  using sounds_t = std::list<sf::Sound>;
  static constexpr int MAX_SOUNDS = 2000;

 public:
  sounds_t::iterator loadSound(const sf::SoundBuffer& buffer);
  sounds_t::iterator loadSound(const sf::Sound& copy);

  sounds_t::iterator playSound(const sf::SoundBuffer& buffer);
  sounds_t::iterator playSound(const sf::Sound& copy);
  sounds_t::iterator playSound(sounds_t::iterator it);
  
  void clearStoppedSounds();
  int playingSounds();

 private:
  sounds_t loaded_sounds;
  sounds_t playing_sounds;
};
