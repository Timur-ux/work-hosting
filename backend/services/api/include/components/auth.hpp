#ifndef AUTH_COMPONENT_HPP_
#define AUTH_COMPONENT_HPP_
#include "userver/server/auth/user_scopes.hpp"
#include "userver/server/handlers/auth/auth_checker_base.hpp"
#include "userver/storages/redis/client_fwd.hpp"
#include "userver/storages/redis/command_control.hpp"
#include <vector>
#pragma once
namespace SERVICE_NAMESPACE {
using namespace userver;
class AuthCheckerBearer final : public server::handlers::auth::AuthCheckerBase {
public:
  using AuthCheckResult = server::handlers::auth::AuthCheckResult;

  AuthCheckerBearer(storages::redis::ClientPtr redis,
                    storages::redis::CommandControl redisCC,
                    std::vector<server::auth::UserScope> requiredScopes);

  [[nodiscard]] AuthCheckResult
  CheckAuth(const server::http::HttpRequest &request,
            server::request::RequestContext &context) const override;

  [[nodiscard]] bool SupportsUserAuth() const noexcept override;

private:
	storages::redis::ClientPtr redis_;
	storages::redis::CommandControl redisCC_;
  const std::vector<server::auth::UserScope> requiredScopes_;
};
} // namespace SERVICE_NAMESPACE
#endif // !AUTH_COMPONENT_HPP_
