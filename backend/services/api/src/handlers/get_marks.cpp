#include "handlers/get_marks.hpp"
#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "schemas/marks.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "userver/utils/string_literal.hpp"
#include "work_hosting/sql_queries.hpp"

#include "userver/components/component_context.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/formats/serialize/common_containers.hpp"
#include <fmt/core.h>

namespace SERVICE_NAMESPACE {
GetMarksHandler::GetMarksHandler(
    const components::ComponentConfig &config,
    const components::ComponentContext &component_context)
    : HttpHandlerJsonBase(config, component_context),
      db_(component_context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {}

GetMarksHandler::Value
GetMarksHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                        const Value &request_json,
                                        RequestContext &context) const {
  request.GetHttpResponse().SetContentType(
      http::content_type::kApplicationJson);
  auto user_id = context.GetData<std::int64_t>("user_id");
  marks::MarksArray response;
  try {
    auto cursor = db_->Execute(storages::postgres::ClusterHostType::kSlave,
                               sql::kFetchStudentMarks, user_id);
    auto marks = cursor.AsSetOf<DenormalizedMark>(storages::postgres::kRowTag);
    using utils::StringLiteral;
    for (auto mark : marks) {
      response.push_back({.work_type = MapToString(mark.work_type),
                          .old_work_number = mark.old_work_number,
                          .mark = MapToString(mark.mark),
                          .status = MapToString(mark.status)});
    }

  } catch (std::exception &e) {
    throw server::handlers::InternalServerError{ExternalBody{fmt::format(
        "Error while fetching student's marks from db. Error: {}", e.what())}};
  }
  return formats::json::ValueBuilder{response}.ExtractValue();
}
} // namespace SERVICE_NAMESPACE
