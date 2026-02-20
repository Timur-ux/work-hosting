#ifndef CHECKING_QUEUE_UPDATER_HPP_
#define CHECKING_QUEUE_UPDATER_HPP_
#include "userver/engine/task/task_with_result.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"
#include "work.hpp"
#include "zmq.hpp"
#pragma once

#include <userver/components/component_base.hpp>
#include <userver/yaml_config/schema.hpp>

namespace SERVICE_NAMESPACE {
using namespace userver;
class CheckingQueueUpdater final : public components::ComponentBase {
public:
  constexpr static std::string_view kName = "checking-queue-updater";

  CheckingQueueUpdater(const components::ComponentConfig &config,
                       const components::ComponentContext &context);
  ~CheckingQueueUpdater();

  static yaml_config::Schema GetStaticConfigSchema();

	static bool IsWorkAlreadyInQueue(storages::postgres::ClusterPtr db, const ::Work &work);

private:
  zmq::socket_t listenSocket_;
  storages::postgres::ClusterPtr db_;

  engine::TaskWithResult<void> task_;
};
} // namespace SERVICE_NAMESPACE

#endif // !CHECKING_QUEUE_UPDATER_HPP_
