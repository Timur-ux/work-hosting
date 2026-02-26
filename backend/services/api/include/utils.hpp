#ifndef UTILS_HPP_
#define UTILS_HPP_
#include <string>
#pragma once

namespace SERVICE_NAMESPACE::utils {

// Calculate 64 bytes hash using blake2b algo
std::string hash(const std::string &msg);

// Generate salt string of random letters with size == n
std::string genSalt(size_t n);

} // namespace SERVICE_NAMESPACE::utils
#endif // !UTILS_HPP_
