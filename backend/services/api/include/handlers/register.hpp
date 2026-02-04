#ifndef HANDLER_REGISTER_HPP_
#define HANDLER_REGISTER_HPP_
#include "userver/storages/postgres/postgres_fwd.hpp"
#pragma once
#include <userver/server/handlers/http_handler_json_base.hpp>
namespace SERVICE_NAMESPACE {
using namespace userver;
class RegisterHandler : public userver::server::handlers::HttpHandlerJsonBase {
public:
  constexpr static std::string_view kName = "register-handler";
  using HttpRequest = server::http::HttpRequest;
  using RequestContext = server::request::RequestContext;

  RegisterHandler(const components::ComponentConfig &config,
                  const components::ComponentContext &component_context);

  Value HandleRequestJsonThrow(const HttpRequest &request,
                               const Value &request_json,
                               RequestContext &context) const override;

private:
	storages::postgres::ClusterPtr db_;
};
} // namespace SERVICE_NAMESPACE
#endif // !HANDLER_REGISTER_HPP_
