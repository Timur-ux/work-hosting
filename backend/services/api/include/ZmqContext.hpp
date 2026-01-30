#ifndef ZMQ_CONTEXT_HPP_
#define ZMQ_CONTEXT_HPP_
#pragma once

#include "zmq.hpp"

namespace SERVICE_NAMESPACE::zmq {
::zmq::context_t & context(); 
} // namespace SERVICE_NAMESPACE

#endif // !ZMQ_CONTEXT_HPP_
