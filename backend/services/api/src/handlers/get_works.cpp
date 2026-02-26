#include "handlers/get_works.hpp"
#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "schemas/work.hpp"
#include "work_hosting/sql_queries.hpp"

#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include <userver/components/component_context.hpp>
#include "userver/formats/json/serialize_container.hpp"

namespace SERVICE_NAMESPACE {
GetWorksHandler::GetWorksHandler(
    const components::ComponentConfig &config,
    const components::ComponentContext &component_context)
    : HttpHandlerJsonBase(config, component_context),
      db_(component_context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {}

GetWorksHandler::Value
GetWorksHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                        const Value &request_json,
                                        RequestContext &context) const {
  request.GetHttpResponse().SetContentType(
      http::content_type::kApplicationJson);

  auto res = db_->Execute(storages::postgres::ClusterHostType::kSlave,
                          sql::kFetchAllWorks);
  auto works = res.AsSetOf<Work>(storages::postgres::kRowTag);

  work::WorksArray response;
  for (auto work : works) {
    auto workTypeLiteral = WorkTypeMapper.TryFindBySecond(work.work_type);
    std::string workType{workTypeLiteral.has_value() ? *workTypeLiteral : ""};
    response.emplace_back(
        work::WorksArrayA{.work_number = work.work_number,
                          .old_work_number = work.old_work_number,
                          .work_type = workType,
                          .theme = work.theme});
  }
  return formats::json::ValueBuilder{response}.ExtractValue();
}
} // namespace SERVICE_NAMESPACE
