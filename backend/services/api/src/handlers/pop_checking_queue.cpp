#include "handlers/pop_checking_queue.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "work_hosting/sql_queries.hpp"

#include "userver/components/component_context.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include <fmt/core.h>

namespace SERVICE_NAMESPACE {
PopCheckingQueueHandler::PopCheckingQueueHandler(
    const components::ComponentConfig &config,
    const components::ComponentContext &component_context)
    : HttpHandlerBase(config, component_context),
      db_(component_context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {}

std::string
PopCheckingQueueHandler::HandleRequestThrow(const HttpRequest &request,
                                            RequestContext &context) const {
  request.GetHttpResponse().SetContentType(http::content_type::kTextPlain);
  try {
    db_->Execute(storages::postgres::ClusterHostType::kMaster,
                 sql::kPopCheckingQueue);
  } catch (std::exception &e) {
    throw server::handlers::InternalServerError{
        ExternalBody{fmt::format("Postgres execute error when poping checking queue: {}", e.what())}};
  }
  std::string response = "";
  return response;
}
} // namespace SERVICE_NAMESPACE
