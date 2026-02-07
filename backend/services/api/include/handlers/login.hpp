#ifndef LOGIN_HANDLER_HPP_
#define LOGIN_HANDLER_HPP_
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/concurrent/background_task_storage.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"
#include "userver/storages/redis/client_fwd.hpp"
#include "userver/storages/redis/command_control.hpp"
#pragma once

#include <userver/server/handlers/http_handler_json_base.hpp>

namespace SERVICE_NAMESPACE {
using namespace userver;
class LoginHandler final : public server::handlers::HttpHandlerJsonBase {
public:
  constexpr static std::string_view kName = "login-handler";

  using HttpRequest = server::http::HttpRequest;
  using RequestContext = server::request::RequestContext;
  using Value = formats::json::Value;

  LoginHandler(const components::ComponentConfig &config,
               const components::ComponentContext &context);

  Value HandleRequestJsonThrow(const HttpRequest &request,
                               const Value &request_json,
                               RequestContext &context) const override;
private:
	storages::postgres::ClusterPtr db_;
	storages::redis::ClientPtr redis_;
	storages::redis::CommandControl redisCC_;
  long tokenTTL_ms_ = 3600000; // 1 hour by default
};
} // namespace SERVICE_NAMESPACE

#endif // !LOGIN_HANDLER_HPP_
