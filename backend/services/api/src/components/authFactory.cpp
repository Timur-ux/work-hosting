#include "components/authFactory.hpp"
#include "components/auth.hpp"
#include "components/authCache.hpp"
#include "userver/server/auth/user_scopes.hpp"
#include <memory>

namespace SERVICE_NAMESPACE {

AuthCheckerFactory::AuthCheckerFactory(
    const components::ComponentContext &context)
    : authCache_(context.FindComponent<AuthCache>()) {}

server::handlers::auth::AuthCheckerBasePtr AuthCheckerFactory::MakeAuthChecker(
    const server::handlers::auth::HandlerAuthConfig &authConfig) const {
  auto scopes = authConfig["scopes"].As<server::auth::UserScopes>({});
  return std::make_shared<AuthCheckerBearer>(authCache_, std::move(scopes));
}
} // namespace SERVICE_NAMESPACE
