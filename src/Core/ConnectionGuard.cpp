#include "Signal.h"

ConnectionGuard::ConnectionGuard(const internal::BaseConnection& conn)
    : conn(conn.clone()) {}

ConnectionGuard& ConnectionGuard::operator=(
    const internal::BaseConnection& conn) {
  assert(!this->conn && "Cannot assign ConnectionGuard more than once");
  this->conn = conn.clone();
  return *this;
}

ConnectionGuard::~ConnectionGuard() { disconnect(); }

void ConnectionGuard::disconnect() {
  if (conn) {
    conn->disconnect();
    delete conn;
    conn = nullptr;
  }
}
