#include "Game.h"

int main() {
  Game& game = Game::instance();
  game.run();
}
