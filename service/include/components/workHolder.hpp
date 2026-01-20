#ifndef COMPONENT_WORK_HOLDER_HPP_
#define COMPONENT_WORK_HOLDER_HPP_
#include <list>
#include <unordered_map>
#include <userver/components/component_base.hpp>
#include <userver/components/component_fwd.hpp>
#include <userver/concurrent/variable.hpp>
#include <userver/engine/shared_mutex.hpp>
#include <userver/engine/task/task_processor_fwd.hpp>
#include <userver/engine/task/task_with_result.hpp>

#include "work.hpp"
#pragma once
namespace SERVICE_NAMESPACE {
class WorkHolder : public userver::components::ComponentBase {
public:
	enum class TaskResult {
		kSucceed,
		kFailed
	};
  static constexpr std::string_view kName = "work-holder";

	WorkHolder(const userver::components::ComponentConfig & config, const userver::components::ComponentContext & context);
	static userver::yaml_config::Schema GetStaticConfigSchema();

	void addTask(const Work & work);
	bool contains(const Work & work) const;

private:
	template<typename T>
	using Variable = userver::concurrent::Variable<T>;

	using task_t = userver::engine::TaskWithResult<TaskResult>;
	Variable<std::list<task_t>> tasks_;
	Variable<std::unordered_map<std::string, std::list<task_t>::iterator>> pointers_;
	userver::concurrent::Variable<size_t, userver::engine::SharedMutex> runningTasks_ = 0;

	userver::engine::TaskProcessor & taskProcessor_;

	size_t maxRunningTasks_ = 1;
	long maxWorkCheckWaitTime_ms = 5000;
};
} // namespace SERVICE_NAMESPACE
#endif // !COMPONENT_WORK_HOLDER_HPP_
