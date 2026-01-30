#ifndef COMPONENT_WORK_HOLDER_HPP_
#define COMPONENT_WORK_HOLDER_HPP_
#include <userver/components/component_base.hpp>
#include <userver/components/component_fwd.hpp>
#include <userver/concurrent/variable.hpp>
#include <userver/engine/shared_mutex.hpp>
#include <userver/engine/task/task_processor_fwd.hpp>
#include <userver/engine/task/task_with_result.hpp>

#include "zmq.hpp"

#include "work.hpp"
#pragma once
namespace SERVICE_NAMESPACE {
class WorkHolder : public userver::components::ComponentBase {
public:
	enum class TaskResult {
		kSucceed,
		kFailed,
		kRunning,
		kUndefined
	};
  static constexpr std::string_view kName = "work-holder";

	WorkHolder(const userver::components::ComponentConfig & config, const userver::components::ComponentContext & context);
	using ComponentBase::GetStaticConfigSchema;

	void addTask(const Work & work);
private:
	zmq::socket_t checkerSocket_;
};
} // namespace SERVICE_NAMESPACE
#endif // !COMPONENT_WORK_HOLDER_HPP_
