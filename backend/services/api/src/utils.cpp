#include "utils.hpp"

#include <cryptopp/blake2.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>

namespace SERVICE_NAMESPACE::utils {
std::string hash(const std::string &msg) {
  using namespace CryptoPP;
  static BLAKE2b hash;

  std::string digest;
  hash.Update((const byte *)msg.data(), msg.size());
  digest.resize(hash.DigestSize());
  hash.Final((byte *)&digest[0]);
  return digest;
}
} // namespace SERVICE_NAMESPACE::utils
