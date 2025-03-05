#include "Action.h"

void Action::update(Action*& action, sf::Time dt) {
  bool update;
  while (action && !(update = action->update(dt))) {
    action->end();
    action = action->next;
    if (action) {
      action->start();
    }
  }
}
