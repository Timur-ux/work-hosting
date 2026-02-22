#include "components/checking_queue_updater.hpp"
#include "ZmqContext.hpp"
#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "userver/components/component_context.hpp"
#include "userver/logging/level.hpp"
#include "userver/logging/log.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "userver/utils/async.hpp"
#include "work.hpp"
#include "work_hosting/sql_queries.hpp"
#include "zmq.hpp"
#include <userver/components/component_config.hpp>
#include <userver/storages/postgres/postgres.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace SERVICE_NAMESPACE {
CheckingQueueUpdater::CheckingQueueUpdater(
    const components::ComponentConfig &config,
    const components::ComponentContext &context)
    : components::ComponentBase(config, context),
      listenSocket_(zmq::context(), ::zmq::socket_type::pull),
      db_(context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {
  std::string addr = config["listen-addr"].As<std::string>();
  int recieveTimeout = config["recieve-timeout-ms"].As<int>();
	std::string taskProcessorName = config["task_processor"].As<std::string>();
	auto& taskProcessor = context.GetTaskProcessor(taskProcessorName);

  listenSocket_.bind(addr);
  listenSocket_.set(::zmq::sockopt::rcvtimeo, recieveTimeout);

  task_ = utils::Async(taskProcessor, "checking-queue-updater-func", [&db = db_,
                                                       &socket =
                                                           listenSocket_]() {
    using namespace userver::server::handlers;
    ::zmq::message_t msg;
    while (true) {
      auto ret = socket.recv(msg);
			if(ret.has_value())
				LOG_INFO() << "Accept request to add to checking queue";
      if (!ret)
        continue;

      try {
        auto workRequest = ::Work::from_string(msg.to_string_view());
        if (IsWorkAlreadyInQueue(db, workRequest))
          continue;

        auto tr = db->Begin("update-checking-queue-if-needed",
                            storages::postgres::ClusterHostType::kMaster, {});

        auto res = tr.Execute(sql::kFindStudent, workRequest.gvName());
        if (!res.Size()) {
          LOG_WARNING() << "Student not found! Work: "
                        << workRequest.to_string();
          continue;
        }
        auto student = res.AsSingleRow<Student>(storages::postgres::kRowTag);

        auto workType = workRequest.type() == ::Work::Type::LR ? "LR" : "KP";
        res = tr.Execute(sql::kFindWork, *WorkTypeMapper.TryFind(workType),
                         int(workRequest.number()));
        if (!res.Size()) {
          LOG_WARNING() << "Work not found! Work: " << workRequest.to_string();
          continue;
        }
        auto workData = res.AsSingleRow<Work>(storages::postgres::kRowTag);

        res = tr.Execute(sql::kAddCheckingQueue, student.id, workData.id);
        if (!res.RowsAffected()) {
          LOG_WARNING() << "Unable to add work to checking queue";
          continue;
        }
				tr.Commit();
				LOG_INFO() << "Work of student " << student.gv_name <<" added to checking queue";
      } catch (std::exception &e) {
        LOG_ERROR() << "Undefinded error while processing message: "
                    << msg.to_string_view();
      }
    }
  });
}

bool CheckingQueueUpdater::IsWorkAlreadyInQueue(
    storages::postgres::ClusterPtr db, const ::Work &work) {

  auto workType = work.type() == ::Work::Type::LR ? "LR" : "KP";
  auto res = db->Execute(storages::postgres::ClusterHostType::kSlave,
                         sql::kIsQueueEntryExists, work.gvName(),
                         *WorkTypeMapper.TryFind(workType), int(work.number()));

  if (res.AsSingleRow<bool>()) {
    LOG_WARNING() << "Work already exists in queue: " << work.to_string();
    return true;
  }
  return false;
}

CheckingQueueUpdater::~CheckingQueueUpdater() {
  task_.RequestCancel();
  task_.BlockingWait();
}

yaml_config::Schema CheckingQueueUpdater::GetStaticConfigSchema() {
  return yaml_config::MergeSchemas<components::ComponentBase>(R"(
type: object
additionalProperties: false
description: Adds works to database's checking queue
properties:
  listen-addr:
    type: string
    description: addres the components will accept messages from checker service on
  recieve-timeout-ms:
    type: integer
    minimum: 1000
    maximum: 10000
    description: awaiting delay in milliseconds for recieving messages(inpact on shutdown time)
  task_processor:
    type: string
    description: name of task processor the checking-queue-updater-func will run on
)");
}
} // namespace SERVICE_NAMESPACE
