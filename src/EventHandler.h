#pragma once

#include <any>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "Core/Signal.h"

class EventHandler {
 public:
  EventHandler() = default;

  template <typename EventT>
  decltype(auto) sink() {
    using T = std::decay_t<EventT>;
    return std::any_cast<Signal<T>&>(
        sinks.try_emplace(typeid(T), std::in_place_type<Signal<T>>)
            .first->second);
  }

 private:
  std::unordered_map<std::type_index, std::any> sinks;
};
