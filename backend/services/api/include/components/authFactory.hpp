#ifndef AUTH_FACTORY_HPP_
#define AUTH_FACTORY_HPP_
#include "components/authCache.hpp"
#include "userver/components/component_context.hpp"
#include "userver/server/handlers/auth/auth_checker_factory.hpp"
#include <string_view>
#pragma once
namespace SERVICE_NAMESPACE {
using namespace userver;
class AuthCheckerFactory final
    : public server::handlers::auth::AuthCheckerFactoryBase {
public:
	static constexpr std::string_view kAuthType = "bearer";

	explicit AuthCheckerFactory(const components::ComponentContext &context);

  server::handlers::auth::AuthCheckerBasePtr MakeAuthChecker(
      const server::handlers::auth::HandlerAuthConfig &) const override;
private:
	AuthCache &authCache_;
};
} // namespace SERVICE_NAMESPACE
#endif // !AUTH_FACTORY_HPP_
