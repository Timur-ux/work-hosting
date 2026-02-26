#include "handlers/get_checking_queue.hpp"
#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "schemas/checkingQueue.hpp"
#include "work_hosting/sql_queries.hpp"

#include "userver/components/component_context.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/formats/serialize/common_containers.hpp"

namespace SERVICE_NAMESPACE {
GetCheckingQueueHandler::GetCheckingQueueHandler(
    const components::ComponentConfig &config,
    const components::ComponentContext &component_context)
    : HttpHandlerJsonBase(config, component_context),
      db_(component_context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {}

GetCheckingQueueHandler::Value
GetCheckingQueueHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                                const Value &request_json,
                                                RequestContext &context) const {
  request.GetHttpResponse().SetContentType(
      http::content_type::kApplicationJson);

  // Some handle logic
  auto res = db_->Execute(storages::postgres::ClusterHostType::kSlave,
                          sql::kFetchCheckingQueue);
	auto checkingQueueItems = res.AsSetOf<CheckingQueueItem>(storages::postgres::kRowTag);

  checkingQueue::CheckingQueueArray response;
	for(auto queueItem : checkingQueueItems) {
		auto workType = WorkTypeMapper.TryFindBySecond(queueItem.work_type).value();
		response.push_back(checkingQueue::CheckingQueueArrayA{
				.gv_name = queueItem.gv_name,
				.work_type = std::string(workType),
				.old_work_number = queueItem.old_work_number
				});
	}
  return formats::json::ValueBuilder{response}.ExtractValue();
}
} // namespace SERVICE_NAMESPACE
