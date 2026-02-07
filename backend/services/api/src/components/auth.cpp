#include "components/auth.hpp"
#include "schemas/auth.hpp"
#include "userver/http/common_headers.hpp"
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

AuthCheckerBearer::AuthCheckResult
AuthCheckerBearer::CheckAuth(const server::http::HttpRequest &request,
                             server::request::RequestContext &context) const {
  const auto &authHeader = request.GetHeader(http::headers::kAuthorization);
  if (authHeader.empty())
    return AuthCheckResult{AuthCheckResult::Status::kTokenNotFound,
                           {},
                           "Empty 'Authorization' header",
                           server::handlers::HandlerErrorCode::kUnauthorized};

  const auto bearerSepIndex = authHeader.find(' ');
  if (bearerSepIndex == std::string::npos ||
      std::string_view(authHeader.data(), bearerSepIndex) != "Bearer")
    return AuthCheckResult{
        AuthCheckResult::Status::kTokenNotFound,
        {},
        "'Authorization' header must have 'Bearer some-token' format",
        server::handlers::HandlerErrorCode::kUnauthorized};

  const server::auth::UserAuthInfo::Ticket token{authHeader.data() +
                                                 bearerSepIndex + 1};

  auto cacheEntryOpt = redis_->Get(token.GetUnderlying(), redisCC_).Get();
  if (!cacheEntryOpt.has_value())
    return AuthCheckResult{AuthCheckResult::Status::kTokenNotFound,
                           {},
                           "Token not found in cache(maybe expired)"};

  auto cacheEntry =
      formats::json::FromString(*cacheEntryOpt).As<auth::AuthCacheEntry>();
  for (const auto &scope : requiredScopes_)
    if (std::find(std::begin(cacheEntry.scopes), std::end(cacheEntry.scopes),
                  scope) == std::end(cacheEntry.scopes))
      return AuthCheckResult{AuthCheckResult::Status::kForbidden,
                             {},
                             "No '" + scope.GetValue() + "' permission"};
  return {};
}

bool AuthCheckerBearer::SupportsUserAuth() const noexcept { return true; }
} // namespace SERVICE_NAMESPACE
