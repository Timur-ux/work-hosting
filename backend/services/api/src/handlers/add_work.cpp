#include "handlers/add_work.hpp"

#include "db/CustomTypesMapping.hpp"
#include "userver/components/component_context.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"

#include "schemas/AddWork.hpp"
#include "work_hosting/sql_queries.hpp"

namespace SERVICE_NAMESPACE {
AddWorkHandler::AddWorkHandler(
    const components::ComponentConfig &config,
    const components::ComponentContext &component_context)
    : HttpHandlerJsonBase(config, component_context),
      db_(component_context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {}

AddWorkHandler::Value
AddWorkHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                       const Value &request_json,
                                       RequestContext &context) const {
  request.GetHttpResponse().SetContentType(
      http::content_type::kApplicationJson);
  auto body = request_json.As<AddWork::AddWorkRequestBody>();
  try {
    db_->Execute(storages::postgres::ClusterHostType::kMaster, sql::kInsertWork,
                 *WorkTypeMapper.TryFind(body.work_type), body.old_work_number,
                 body.theme.value_or(""));
  } catch (std::exception &e) {
    throw server::handlers::InternalServerError(ExternalBody{e.what()});
  }

  return formats::json::ValueBuilder{""}.ExtractValue();
}
} // namespace SERVICE_NAMESPACE
