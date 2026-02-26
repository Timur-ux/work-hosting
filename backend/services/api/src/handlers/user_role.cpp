#include "handlers/user_role.hpp"
#include "components/auth.hpp"
#include "schemas/auth.hpp"
#include "userver/components/component_context.hpp"
#include "userver/http/common_headers.hpp"
#include "userver/http/predefined_header.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/storages/redis/component.hpp"

namespace SERVICE_NAMESPACE {
UserRoleHandler::UserRoleHandler(
    const components::ComponentConfig &config,
    const components::ComponentContext &component_context)
    : HttpHandlerBase(config, component_context),
      redis_(component_context.FindComponent<components::Redis>("redis-cache")
                 .GetClient("api_cache_db")) {}

std::string UserRoleHandler::HandleRequestThrow(const HttpRequest &request,
                                                RequestContext &context) const {
  request.GetHttpResponse().SetContentType(http::content_type::kTextPlain);
  auto &authHeader = request.GetHeader(http::headers::kAuthorization);
  auth::AuthCacheEntry cache;
  try {
    cache = AuthCheckerBearer::GetAuthByToken(authHeader, redis_, redisCC_);
  } catch (AuthCheckerBearer::AuthCheckResult &authError) {
    throw server::handlers::Unauthorized(
        ExternalBody{authError.ext_reason.value_or("No reason text provided")});
  }
  if (cache.scopes.size() != 1)
    throw server::handlers::InternalServerError{ExternalBody{fmt::format(
        "Expected 1 scope for user. Given {}", cache.scopes.size())}};

  return cache.scopes[0];
}
} // namespace SERVICE_NAMESPACE
