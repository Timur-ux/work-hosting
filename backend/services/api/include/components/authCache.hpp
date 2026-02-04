#ifndef AUTH_CACHE_HPP_
#define AUTH_CACHE_HPP_
#include "userver/cache/base_postgres_cache.hpp"
#include "userver/crypto/algorithm.hpp"
#include "userver/storages/postgres/io/chrono.hpp"
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#pragma once
#include <userver/server/auth/user_auth_info.hpp>

namespace SERVICE_NAMESPACE {
struct UserDbInfo {
  userver::server::auth::UserAuthInfo::Ticket token;
  std::int64_t user_id;
  std::vector<std::string> scopes;
  std::string username;
	userver::storages::postgres::TimePointTz updated;
};

struct AuthCachePolicy {
  static constexpr std::string_view kName = "auth-pg-cache";

  using ValueType = UserDbInfo;
  static constexpr auto kKeyMember = &UserDbInfo::token;
  static constexpr const char *kQuery =
      "SELECT token, user_id, scopes, username, updated FROM auth_schema.tokens";
  static constexpr const char *kUpdatedField = "updated";
  using UpdatedFieldType = userver::storages::postgres::TimePointTz;

  using CacheContainer = std::unordered_map<
      userver::server::auth::UserAuthInfo::Ticket, UserDbInfo,
      std::hash<userver::server::auth::UserAuthInfo::Ticket>,
      userver::crypto::algorithm::StringsEqualConstTimeComparator>;
};

using AuthCache = userver::components::PostgreCache<AuthCachePolicy>;
} // namespace SERVICE_NAMESPACE
#endif // !AUTH_CACHE_HPP_
