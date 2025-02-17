#include "Signal.h"

inline ConnectionGuard::ConnectionGuard(const internal::BaseConnection& conn)
    : conn(conn.clone()) {}

inline ConnectionGuard& ConnectionGuard::operator=(
    const internal::BaseConnection& conn) {
  assert(!this->conn && "Cannot assign ConnectionGuard more than once");
  this->conn = conn.clone();
  return *this;
}

inline ConnectionGuard::~ConnectionGuard() { disconnect(); }

inline void ConnectionGuard::disconnect() {
  if (conn) {
    conn->disconnect();
    delete conn;
  }
}
