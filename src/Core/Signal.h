#pragma once

#include <cassert>
#include <functional>
#include <list>

// NOTE: reentrability stuff supported:
// * calling other slots inside a slot
// * disconnection of other slots
// * disconnection of own slot
// * destruction of the signal itself
// * throwing exceptions inside a slot

template <typename... Args>
class Signal {
 public:
  using slot_t = std::function<void(Args...)>;
  using slots_t = std::list<slot_t>;

  class Connection {
   public:
    Connection() = default;

    Connection(Connection&& other) : signal(other.signal), it(other.it) {}

    Connection& operator=(Connection&& other) {
      signal = other.signal;
      it = other.it;
      return *this;
    }

    Connection(Signal* signal, typename slots_t::iterator it)
        : signal(signal), it(it) {
      assert(it != signal->slots.end());
    }

    void disconnect() {
      if (signal->inside_emit)
        *it = slot_t();
      else
        signal->slots.erase(it);
    }

    operator bool() { return signal; }

   private:
    Signal* signal = nullptr;
    typename slots_t::iterator it;
  };

 public:
  Signal() = default;

  // NOTE: This is a big fat hack to work around
  //       std::any's CopyConstructible requirement
  //       because it always defines a copy constructor
  // Signal(const Signal&) {
  //   assert(false && "signals cannot be copy-constructed");
  // }

  ~Signal() {
    if (destroyed) *destroyed = true;
  }

  Connection subscribe(slot_t slot) {
    assert(slot);
    return {this, slots.insert(slots.end(), slot)};
  }

  void operator()(Args... args) const {
    inside_emit++;
    bool* destroyed_old = destroyed;
    bool is_destroyed = false;
    destroyed = &is_destroyed;

    try {
      for (auto it = slots.begin(); it != slots.end(); ++it) {
        if (!*it) continue;
        (*it)(std::forward<Args>(args)...);
        if (!is_destroyed) continue;
        if (destroyed_old) *destroyed_old = true;
        return;
      }
    } catch (...) {
      destroyed = destroyed_old;
      leave_emit();
      throw;
    }

    destroyed = destroyed_old;
    leave_emit();
  }

 private:
  void leave_emit() const noexcept {
    inside_emit--;
    if (inside_emit != 0) return;
    for (auto it = slots.begin(); it != slots.end();) {
      if (*it)
        ++it;
      else
        it = slots.erase(it);
    }
  }

 private:
  mutable slots_t slots;
  mutable size_t inside_emit = 0;
  mutable bool* destroyed = nullptr;
};
