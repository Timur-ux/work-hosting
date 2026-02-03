#ifndef AUTH_COMPONENT_HPP_
#define AUTH_COMPONENT_HPP_
#include "components/authCache.hpp"
#include "userver/server/auth/user_scopes.hpp"
#include "userver/server/handlers/auth/auth_checker_base.hpp"
#include "userver/storages/postgres/component.hpp"
#include <vector>
#pragma once
namespace SERVICE_NAMESPACE {
using namespace userver;
class AuthCheckerBearer final : public server::handlers::auth::AuthCheckerBase {
public:
  using AuthCheckResult = server::handlers::auth::AuthCheckResult;

  AuthCheckerBearer(const AuthCache &authCache,
                    std::vector<server::auth::UserScope> requiredScopes);

  [[nodiscard]] AuthCheckResult
  CheckAuth(const server::http::HttpRequest &request,
            server::request::RequestContext &context) const override;

  [[nodiscard]] bool SupportsUserAuth() const noexcept override;

private:
	long tokenTTL_ms_ = 3600000; // 1 hour by default
  const AuthCache &authCache_;
  const std::vector<server::auth::UserScope> requiredScopes_;
};
} // namespace SERVICE_NAMESPACE
#endif // !AUTH_COMPONENT_HPP_
