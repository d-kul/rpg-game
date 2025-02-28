#include "Action.h"

void AbstractAction::update(std::unique_ptr<AbstractAction> &action,
                            sf::Time dt) {
  bool update;
  while (action && !(update = action->update(dt))) {
    action->end();
    action = std::move(action->next);
    if (action) {
      action->start();
    }
  }
}

std::unique_ptr<AbstractAction> AbstractAction::clone() const {
  auto result = cloneImpl();
  result->next = next->clone();
  return result;
}
