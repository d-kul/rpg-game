#pragma once

#include <any>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "Core/Signal.h"

class EventManager {
 public:
  EventManager() = default;

  template <typename EventT>
  decltype(auto) sink() {
    using T = std::decay_t<EventT>;
    return std::any_cast<Signal<T>&>(
        sinks.try_emplace(typeid(T), std::in_place_type<Signal<T>>)
            .first->second);
  }

  template <typename EventT>
  decltype(auto) subscribe(typename Signal<std::decay_t<EventT>>::slot_t slot) {
    using T = std::decay_t<EventT>;
    return sink<T>().subscribe(slot);
  }

  template <typename EventT, typename Class>
  decltype(auto) bind(void (Class::*ptr)(std::decay_t<EventT>),
                      Class* instance) {
    using T = std::decay_t<EventT>;
    return sink<T>().bind(ptr, instance);
  }

 private:
  std::unordered_map<std::type_index, std::any> sinks;
};
