#include "components/workHolder.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <userver/components/component.hpp>
#include <userver/components/component_base.hpp>
#include <userver/engine/exception.hpp>
#include <userver/engine/sleep.hpp>
#include <userver/logging/log.hpp>
#include <userver/utils/async.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/yaml_config/schema.hpp>

namespace SERVICE_NAMESPACE {

WorkHolder::WorkHolder(const userver::components::ComponentConfig &config,
                       const userver::components::ComponentContext &context)
    : ComponentBase(config, context),
      taskProcessor_(context.GetTaskProcessor(
          config["task-processor"].As<std::string_view>())),
      maxRunningTasks_(config["max-running-tasks"].As<size_t>()),
      maxWorkCheckWaitTime_ms(
          config["max-work-check-wait-time-ms"].As<size_t>()) {}

userver::yaml_config::Schema WorkHolder::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<userver::components::ComponentBase>(
      R"(
type: object
description: Holds task with works checking
additionalProperties: false
properties:
  task-processor:
    type: string
    description: name of the task processor the tasks will run on
  max-running-tasks:
		type: integer
		description: maximum quantity simultaniously running tasks
	max-work-check-wait-time-ms:
		type: integer
		minimum: 5000
		description: maximum amount time to check the work in milliseconds
	)");
}

void WorkHolder::addTask(const Work &work) {
  if (contains(work))
    return;
  using namespace std::chrono;

  auto tasks = tasks_.Lock();
  auto it = tasks->insert(
      std::end(*tasks),
      userver::utils::Async(
          taskProcessor_, std::format("check-work[{}]", work.to_string()),
          [&work, this]() {
            int retCode;
            auto start = std::chrono::system_clock::now();
            std::thread thread{[&work, &retCode]() {
              std::cout.flush();
              // TODO: Add logic for externally point to script file
              retCode = std::system(
                  std::format("$HOME/scripts/checkWork.sh {} {} {}",
                              work.typeAsString(), work.number(), work.gvName())
                      .c_str());
            }};
            auto end = system_clock::now();
            auto timeSpan_ms = duration_cast<milliseconds>(end - start).count();
            while (!thread.joinable() &&
                   timeSpan_ms < maxWorkCheckWaitTime_ms) {
              userver::engine::InterruptibleSleepFor(milliseconds{5000});
              end = system_clock::now();
              timeSpan_ms = duration_cast<milliseconds>(end - start).count();
            }
            if (thread.joinable())
              return thread.join(), WEXITSTATUS(retCode) == 0
                                        ? TaskResult::kSucceed
                                        : TaskResult::kFailed;
            return thread.detach(), TaskResult::kFailed;
          }));
  (*pointers_.Lock())[work.to_string()] = it;
}

bool WorkHolder::contains(const Work &work) const {
  return (*pointers_.Lock()).contains(work.to_string());
}

WorkHolder::TaskResult WorkHolder::getStatus(const Work &work) {
  auto pointers = pointers_.Lock();
  if (!pointers->contains(work.to_string()))
    return TaskResult::kUndefined;
  auto workIt = (*pointers)[work.to_string()];

  if (!workIt->IsFinished())
    return TaskResult::kRunning;

  try {
    return workIt->Get();
  } catch (userver::engine::WaitInterruptedException &e) {
    LOG_WARNING()
        << "WaitInterrupt error while getting work check task result: "
        << e.what();
  } catch (userver::engine::TaskCancelledException &e) {
    LOG_WARNING()
        << "TaskCancelled error while getting work check task result: "
        << e.what();
  }
  pointers->erase(work.to_string());
  return TaskResult::kFailed;
}
} // namespace SERVICE_NAMESPACE
