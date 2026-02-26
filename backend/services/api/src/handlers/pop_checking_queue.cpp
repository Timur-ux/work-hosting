#include "handlers/pop_checking_queue.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "userver/components/component_context.hpp"
#include "userver/formats/json/serialize.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/component.hpp"

#include "schemas/popQueue.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "work.hpp"
#include "work_hosting/sql_queries.hpp"

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
  auto &action = request.GetArg("action");
  if (action.empty())
    throw server::handlers::InternalServerError(ExternalBody{
        "Query argument [action=accept/reject/requeue] not specified!"});
  try {
    auto queueItemData = formats::json::FromString(request.RequestBody())
                             .As<popQueue::PopQueueRequestBody>();
    auto tr = db_->Begin("pop-work-from-queue",
                         storages::postgres::ClusterHostType::kMaster, {});
    auto res = tr.Execute(sql::kFindQueueEntry, queueItemData.gv_name,
                          *WorkTypeMapper.TryFind(queueItemData.work_type),
                          queueItemData.old_work_number);
    auto queueItem =
        res.AsSingleRow<CheckingQueueItem>(storages::postgres::kRowTag);
    if (action == "accept")
      res = tr.Execute(sql::kAcceptWork, queueItem.queue_id);
    else if (action == "reject")
      res = tr.Execute(sql::kRejectWork, queueItem.queue_id);
    else if (action == "requeue")
      res = tr.Execute(sql::kRequeueWork, queueItem.queue_id);
    else
      throw server::handlers::ClientError(
          ExternalBody{fmt::format("Given action: {} not valid!", action)});
    if (res.RowsAffected() == 0) {
      LOG_ERROR() << "Can't insert work data to another checking queue table: "
                  << WorkTypeMapper.TryFindBySecond(queueItem.work_type) << ' '
                  << queueItem.old_work_number << ' ' << queueItem.gv_name
                  << "; Queue entry id: " << queueItem.queue_id;
      throw server::handlers::InternalServerError{
          ExternalBody{"Error while processing work"}};
    }

    db_->Execute(storages::postgres::ClusterHostType::kMaster,
                 sql::kPopCheckingQueue, queueItem.queue_id);
    tr.Commit();
  } catch (server::handlers::CustomHandlerException &e) {
    throw;
  } catch (std::exception &e) {
    throw server::handlers::InternalServerError{
        ExternalBody{fmt::format("Error: {}", e.what())}};
  }
  std::string response = "Ok";
  return response;
}
} // namespace SERVICE_NAMESPACE
