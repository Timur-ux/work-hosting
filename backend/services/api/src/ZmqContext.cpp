#include "ZmqContext.hpp"

namespace SERVICE_NAMESPACE::zmq {
::zmq::context_t &context() {
  static ::zmq::context_t context{};
  return context;
}
} // namespace SERVICE_NAMESPACE::zmq
