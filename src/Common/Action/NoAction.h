#pragma once

#include "Action.h"

class NoAction : public Action {
 public:
  void perform() override {}
};
