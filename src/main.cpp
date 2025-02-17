#include "Game.h"

int main() {
  Game& game = Game::getInstance();
  game.run();
}
