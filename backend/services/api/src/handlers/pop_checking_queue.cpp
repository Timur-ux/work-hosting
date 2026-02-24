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
      AcceptWork(queueItem, tr);
    else if (action == "reject")
      RejectWork(queueItem, tr);
    else if (action == "requeue")
      RequeueWork(queueItem, tr);
    else
      throw server::handlers::ClientError(
          ExternalBody{fmt::format("Given action: {} not valid!", action)});

    db_->Execute(storages::postgres::ClusterHostType::kMaster,
                 sql::kPopCheckingQueue, queueItem.queue_id);
  } catch (std::exception &e) {
    throw server::handlers::InternalServerError{ExternalBody{fmt::format(
        "Postgres execute error when poping checking queue: {}", e.what())}};
  }
  std::string response = "Ok";
  return response;
}

void PopCheckingQueueHandler::AcceptWork(
    const CheckingQueueItem &queuedWork,
    storages::postgres::Transaction &tr) const {
  auto res = tr.Execute(sql::kAcceptWork, queuedWork.queue_id);
  if (res.RowsAffected() == 0)
    throw server::handlers::InternalServerError(
        ExternalBody{"Accepting work failed"});
}

void PopCheckingQueueHandler::RejectWork(
    const CheckingQueueItem &queuedWork,
    storages::postgres::Transaction &tr) const {
  auto res = tr.Execute(sql::kRejectWork, queuedWork.queue_id);
  if (res.RowsAffected() == 0)
    throw server::handlers::InternalServerError(
        ExternalBody{"Accepting work failed"});
}

void PopCheckingQueueHandler::RequeueWork(
    const CheckingQueueItem &queuedWork,
    storages::postgres::Transaction &tr) const {
  auto res = tr.Execute(sql::kRequeueWork, queuedWork.queue_id);
  if (res.RowsAffected() == 0)
    throw server::handlers::InternalServerError(
        ExternalBody{"Accepting work failed"});
}
} // namespace SERVICE_NAMESPACE
