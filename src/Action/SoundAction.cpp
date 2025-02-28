#include "SoundAction.h"

#include "Game.h"

void SoundAction::start() {
  if (buffer) {
    Game::getAudioManager().playSound(*buffer);
  }
}
