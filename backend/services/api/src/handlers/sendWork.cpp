#include "handlers/sendWork.hpp"
#include "CheckerRequest.hpp"
#include "components/workHolder.hpp"
#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "schemas/check.hpp"
#include "userver/http/content_type.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "userver/storages/redis/component.hpp"
#include "userver/yaml_config/merge_schemas.hpp"
#include "work.hpp"
#include "work_hosting/sql_queries.hpp"
#include <chrono>
#include <fmt/core.h>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/serialize_container.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/storages/redis/client.hpp>

namespace impl {
Work workMap(const check::WorkRequest &requestBody,
             const std::string &username) {
  Work::Type type =
      requestBody.work_type == "LR" ? Work::Type::LR : Work::Type::KP;
  if (!requestBody.old_work_number)
    throw userver::server::handlers::ResourceNotFound(
        userver::server::handlers::ExternalBody{"Work number not set!"});

  return Work(type, static_cast<unsigned short>(requestBody.old_work_number),
              username);
}
} // namespace impl

namespace SERVICE_NAMESPACE {
SendWorkHandler::SendWorkHandler(
    const userver::components::ComponentConfig &config,
    const userver::components::ComponentContext &context)
    : HttpHandlerJsonBase(config, context),
      workHolder_(context.FindComponent<WorkHolder>()),
      db_(context.FindComponent<userver::components::Postgres>("pg-database")
              .GetCluster()),
      redis_(context.FindComponent<userver::components::Redis>("redis-cache")
                 .GetClient("api_cache_db")) {
  send_timeout_ms_ = config["send-timeout-ms"].As<long>();
}

userver::yaml_config::Schema SendWorkHandler::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<HttpHandlerJsonBase>(R"(
type: object
additionalProperties: false
description: proxy for work in queue to check
properties:
  send-timeout-ms:
    type: integer
    description: timeout in milliseconds to send work from one student
)");
}

SendWorkHandler::Value
SendWorkHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                        const Value &request_json,
                                        RequestContext &context) const {
  using namespace userver::server::handlers;
  using namespace userver::storages::postgres;
  request.GetHttpResponse().SetContentType(
      userver::http::content_type::kApplicationJson);

  const auto &username = context.GetData<std::string>("username");
  auto requestBody = request_json.As<check::WorkRequest>();
  ::Work work = ::impl::workMap(requestBody, username);

	if(redis_->Get(username, redisCC_).Get().has_value())
    throw ConflictError{ExternalBody{fmt::format("Wait for {} milliseconds since last send", send_timeout_ms_)}};

  auto res = db_->Execute(
      ClusterHostType::kSlave, sql::kIsQueueEntryExists, work.gvName(),
      *WorkTypeMapper.TryFind(work.typeAsString()), int(work.number()));
  if (res.AsSingleRow<bool>())
    throw ConflictError{ExternalBody{"Work already in checking queue"}};
  res = db_->Execute(ClusterHostType::kSlave, sql::kFindStudent, work.gvName());
  auto student = res.AsSingleRow<Student>(kRowTag);

  workHolder_.addTask(CheckerRequest{.work = work,
                                     .first_name = student.first_name,
                                     .last_name = student.last_name,
                                     .father_name = student.father_name,
                                     .group_number = student.group_number,
                                     .in_group_order = student.in_group_order});
	redis_->Set(username, "", std::chrono::milliseconds(send_timeout_ms_), redisCC_).IgnoreResult();
  return userver::formats::json::ValueBuilder{
      check::SendWorkResponse{"Ok, added to check queue"}}
      .ExtractValue();
}

} // namespace SERVICE_NAMESPACE
