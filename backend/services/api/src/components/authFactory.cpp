#include "components/authFactory.hpp"
#include "components/auth.hpp"
#include "userver/logging/log.hpp"
#include "userver/server/auth/user_scopes.hpp"
#include "userver/storages/redis/component.hpp"
#include <memory>

namespace SERVICE_NAMESPACE {

AuthCheckerFactory::AuthCheckerFactory(
    const components::ComponentContext &context)
    : redis_(context.FindComponent<components::Redis>("redis-cache")
                 .GetClient("api_cache_db")) {}

server::handlers::auth::AuthCheckerBasePtr AuthCheckerFactory::MakeAuthChecker(
    const server::handlers::auth::HandlerAuthConfig &authConfig) const {
  auto scopes = authConfig["scopes"].As<server::auth::UserScopes>({});
  return std::make_shared<AuthCheckerBearer>(redis_, redisCC_,
                                             std::move(scopes));
}
} // namespace SERVICE_NAMESPACE
