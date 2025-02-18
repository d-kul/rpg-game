#pragma once

#include <SFML/Audio/Sound.hpp>
#include <list>

class SoundManager {
 public:
  void playSound() {

  }
 private:
  std::list<sf::Sound> sounds;
};
