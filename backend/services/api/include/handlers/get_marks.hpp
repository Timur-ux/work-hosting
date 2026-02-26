#ifndef GET_MARKS_HANDLER_HPP_
#define GET_MARKS_HANDLER_HPP_
#include "userver/storages/postgres/postgres_fwd.hpp"
#pragma once

#include <userver/server/handlers/http_handler_json_base.hpp>

namespace SERVICE_NAMESPACE {
using namespace userver;
class GetMarksHandler final : public server::handlers::HttpHandlerJsonBase {
public:
  constexpr static std::string_view kName = "get-marks-handler";

  using HttpRequest = server::http::HttpRequest;
  using RequestContext = server::request::RequestContext;
  using Value = formats::json::Value;

  GetMarksHandler(const components::ComponentConfig &config,
                  const components::ComponentContext &component_context);

  Value HandleRequestJsonThrow(const HttpRequest &request,
                               const Value &request_json,
                               RequestContext &context) const override;
private:
	storages::postgres::ClusterPtr db_;
};
} // namespace SERVICE_NAMESPACE

#endif // !GET_MARKS_HANDLER_HPP_
