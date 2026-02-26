#ifndef USER_ROLE_HANDLER_HPP_
#define USER_ROLE_HANDLER_HPP_
#include "userver/storages/redis/client_fwd.hpp"
#include "userver/storages/redis/command_control.hpp"
#pragma once

#include <userver/server/handlers/http_handler_base.hpp>

namespace SERVICE_NAMESPACE {
using namespace userver;
class UserRoleHandler final : public server::handlers::HttpHandlerBase {
public:
	constexpr static std::string_view kName = "user-role-handler";

	using HttpRequest = server::http::HttpRequest;
	using RequestContext = server::request::RequestContext;
	
	UserRoleHandler(const components::ComponentConfig &config,
                  const components::ComponentContext &component_context);

	std::string HandleRequestThrow(const HttpRequest &request,
                               RequestContext &context) const override;
private:
	storages::redis::ClientPtr redis_;
	storages::redis::CommandControl redisCC_;
};
} // !SERVICE_NAMESPACE

#endif // !USER_ROLE_HANDLER_HPP_
