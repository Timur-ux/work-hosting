#ifndef LOGIN_HANDLER_HPP_
#define LOGIN_HANDLER_HPP_
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/concurrent/background_task_storage.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"
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
	~LoginHandler();
private:
	storages::postgres::ClusterPtr db_;

	concurrent::BackgroundTaskStorage bts_;
};
} // namespace SERVICE_NAMESPACE

#endif // !LOGIN_HANDLER_HPP_
