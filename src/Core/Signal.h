#pragma once

#include <cassert>
#include <functional>
#include <list>
#include <type_traits>

namespace internal {
class BaseConnection;
}  // namespace internal

class ConnectionGuard {
 public:
  ConnectionGuard() = default;
  ConnectionGuard(const internal::BaseConnection& conn);
  ConnectionGuard& operator=(const internal::BaseConnection& conn);
  ~ConnectionGuard();
  void disconnect();

 private:
  internal::BaseConnection* conn = nullptr;
};

namespace internal {
class BaseConnection {
 public:
  virtual ~BaseConnection() {}
  virtual void disconnect() = 0;

 protected:
  virtual BaseConnection* clone() const = 0;
  friend class ConnectionGuard;
};
}  // namespace internal

// NOTE(des): reentrability stuff supported:
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

  class Connection : public internal::BaseConnection {
   public:
    Connection() = default;

    Connection(const Connection& other) : signal(other.signal), it(other.it) {}

    Connection& operator=(const Connection& other) {
      signal = other.signal;
      it = other.it;
      return *this;
    }

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
    BaseConnection* clone() const { return new Connection(*this); }

    Signal* signal = nullptr;
    typename slots_t::iterator it;
  };

 public:
  Signal() = default;

  ~Signal() {
    if (destroyed) *destroyed = true;
  }

  Connection subscribe(slot_t slot) {
    assert(slot);
    return {this, slots.insert(slots.end(), slot)};
  }

  template <typename Class, typename = std::enable_if<sizeof...(Args) <= 5>>
  Connection bind(void (Class::*ptr)(Args...), Class* instance) {
    using namespace std::placeholders;
    assert(instance);
    if constexpr (sizeof...(Args) == 0) {
      return subscribe(std::bind(ptr, instance));
    } else if constexpr (sizeof...(Args) == 1) {
      return subscribe(std::bind(ptr, instance, _1));
    } else if constexpr (sizeof...(Args) == 2) {
      return subscribe(std::bind(ptr, instance, _1, _2));
    } else if constexpr (sizeof...(Args) == 3) {
      return subscribe(std::bind(ptr, instance, _1, _2, _3));
    } else if constexpr (sizeof...(Args) == 4) {
      return subscribe(std::bind(ptr, instance, _1, _2, _3, _4));
    } else {
      return subscribe(std::bind(ptr, instance, _1, _2, _3, _4, _5));
    }
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
