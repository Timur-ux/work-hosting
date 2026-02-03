#include "components/auth.hpp"
#include "components/authCache.hpp"
#include "userver/http/common_headers.hpp"
#include "userver/logging/log.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/utils/datetime_light.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <string>
#include <string_view>

namespace SERVICE_NAMESPACE {
AuthCheckerBearer::AuthCheckerBearer(
    const AuthCache &authCache,
    std::vector<server::auth::UserScope> requiredScopes)
    : authCache_(authCache), requiredScopes_(requiredScopes) {
  const char *tokenTTLenv = getenv("TOKEN_TTL");
  if (!tokenTTLenv) {
    LOG_INFO() << "TOKEN_TTL env not specified, so i use default value: "
               << tokenTTL_ms_ << "ms";
    return;
  }
  long long tokenTTL = -1;
  try {
    tokenTTL = std::stoll(tokenTTLenv);
  } catch (std::exception &e) {
    LOG_WARNING() << "Error while parsing TOKEN_TTL value: " << e.what()
                  << "\nSo i use default value: " << tokenTTL_ms_ << "ms";
    return;
  }
  if (tokenTTL <= 0) {
    LOG_WARNING() << "Given TOKEN_TTL is not positive: " << tokenTTL
                  << "\nIt must be positive, so i use default value: "
                  << tokenTTL_ms_ << "ms";
    return;
  }
  tokenTTL_ms_ = tokenTTL;
  LOG_INFO() << "Using token ttl: " << tokenTTL_ms_ << "ms";
}

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
  const auto cacheSnapshot = authCache_.Get();

  auto it = cacheSnapshot->find(token);
  if (it == cacheSnapshot->end())
    return AuthCheckResult{AuthCheckResult::Status::kForbidden};

  const UserDbInfo &dbInfo = it->second;
  for (const auto &scope : requiredScopes_)
    if (std::find(std::begin(dbInfo.scopes), std::end(dbInfo.scopes), scope) ==
        std::end(dbInfo.scopes))
      return AuthCheckResult{AuthCheckResult::Status::kForbidden,
                             {},
                             "No '" + scope.GetValue() + "' permission"};

  const auto liveTime_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          utils::datetime::Now() - dbInfo.updated.GetUnderlying())
          .count();
  if (liveTime_ms > tokenTTL_ms_)
    return AuthCheckResult{
        AuthCheckResult::Status::kForbidden, {}, "Token expired"};

  context.SetData("gvName", dbInfo.gvName);
  return {};
}

bool AuthCheckerBearer::SupportsUserAuth() const noexcept { return true; }
} // namespace SERVICE_NAMESPACE
