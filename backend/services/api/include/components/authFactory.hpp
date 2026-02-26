#ifndef AUTH_FACTORY_HPP_
#define AUTH_FACTORY_HPP_
#include "userver/components/component_context.hpp"
#include "userver/server/handlers/auth/auth_checker_factory.hpp"
#include "userver/storages/redis/client_fwd.hpp"
#include "userver/storages/redis/command_control.hpp"
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
	storages::redis::ClientPtr redis_;
	storages::redis::CommandControl redisCC_;
};
} // namespace SERVICE_NAMESPACE
#endif // !AUTH_FACTORY_HPP_
