#include "handlers/student_profile.hpp"
#include "db/schemas.hpp"
#include "schemas/studentProfile.hpp"
#include "userver/components/component_context.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/http/content_type.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "work_hosting/sql_queries.hpp"
#include <userver/storages/postgres/cluster.hpp>

namespace SERVICE_NAMESPACE {

StudentProfileHandler::StudentProfileHandler(
    const components::ComponentConfig &config,
    const components::ComponentContext &component_context)
    : HttpHandlerJsonBase(config, component_context),
      db_(component_context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {}

StudentProfileHandler::Value
StudentProfileHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                              const Value &request_json,
                                              RequestContext &context) const {
  request.GetHttpResponse().SetContentType(
      http::content_type::kApplicationJson);
  try {
    auto username = context.GetData<std::string>("username");
    auto cursor =
        db_->Execute(storages::postgres::ClusterHostType::kSlave,
                     sql::kFindStudent, username);
    LOG_DEBUG() << "student found";
    auto studentInfo = cursor.AsSingleRow<Student>(storages::postgres::kRowTag);

    studentProfile::GetStudentProfileResponseBody responseBody{
        .gv_name = studentInfo.gv_name,
        .first_name = studentInfo.first_name,
        .last_name = studentInfo.last_name,
        .father_name = studentInfo.father_name,
        .email = studentInfo.email,
        .group_number = studentInfo.group_number,
        .in_group_order = studentInfo.in_group_order};

    auto builder = formats::json::ValueBuilder{responseBody};
    LOG_DEBUG() << "Builder created";
    return builder.ExtractValue();
  } catch (std::exception &e) {
    LOG_ERROR() << "Error: " << e.what();
    throw server::handlers::InternalServerError{};
  }
}
} // namespace SERVICE_NAMESPACE
