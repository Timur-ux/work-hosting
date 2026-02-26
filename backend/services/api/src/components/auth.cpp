#include "components/auth.hpp"
#include "schemas/auth.hpp"
#include "userver/http/common_headers.hpp"
#include "userver/logging/log.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/storages/redis/client.hpp"
#include <cstdlib>
#include <string>
#include <string_view>

namespace SERVICE_NAMESPACE {
AuthCheckerBearer::AuthCheckerBearer(
    storages::redis::ClientPtr redis, storages::redis::CommandControl redisCC,
    std::vector<server::auth::UserScope> requiredScopes)
    : redis_(redis), redisCC_(redisCC), requiredScopes_(requiredScopes) {}

auth::AuthCacheEntry
AuthCheckerBearer::GetAuthByToken(std::string authHeader,
                                  storages::redis::ClientPtr redis,
                                  storages::redis::CommandControl redisCC) {
  if (authHeader.empty())
    throw AuthCheckResult{AuthCheckResult::Status::kTokenNotFound,
                          {},
                          "Empty 'Authorization' header",
                          server::handlers::HandlerErrorCode::kUnauthorized};

  const auto bearerSepIndex = authHeader.find(' ');
  if (bearerSepIndex == std::string::npos ||
      std::string_view(authHeader.data(), bearerSepIndex) != "Bearer")
    throw AuthCheckResult{
        AuthCheckResult::Status::kTokenNotFound,
        {},
        "'Authorization' header must have 'Bearer some-token' format",
        server::handlers::HandlerErrorCode::kUnauthorized};

  const server::auth::UserAuthInfo::Ticket token{authHeader.data() +
                                                 bearerSepIndex + 1};

  auto cacheEntryOpt = redis->Get(token.GetUnderlying(), redisCC).Get();
  if (!cacheEntryOpt.has_value())
    throw AuthCheckResult{AuthCheckResult::Status::kTokenNotFound,
                          {},
                          "Token not found in cache(maybe expired)"};

  auto cacheEntry =
      formats::json::FromString(*cacheEntryOpt).As<auth::AuthCacheEntry>();
  return cacheEntry;
}

AuthCheckerBearer::AuthCheckResult
AuthCheckerBearer::CheckAuth(const server::http::HttpRequest &request,
                             server::request::RequestContext &context) const {
  const auto &authHeader = request.GetHeader(http::headers::kAuthorization);

	auth::AuthCacheEntry cacheEntry;
 	cacheEntry = GetAuthByToken(authHeader, redis_, redisCC_);
  for (const auto &scope : requiredScopes_)
    if (std::find(std::begin(cacheEntry.scopes), std::end(cacheEntry.scopes),
                  scope) == std::end(cacheEntry.scopes))
      return AuthCheckResult{AuthCheckResult::Status::kForbidden,
                             {},
                             "No '" + scope.GetValue() + "' permission"};
  context.SetData<std::string>("username", cacheEntry.username);
  context.SetData<std::int64_t>("user_id", cacheEntry.user_id);
  return {};
}

bool AuthCheckerBearer::SupportsUserAuth() const noexcept { return true; }
} // namespace SERVICE_NAMESPACE
