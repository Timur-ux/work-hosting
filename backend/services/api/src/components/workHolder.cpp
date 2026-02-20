#include "components/workHolder.hpp"
#include "Serializers.hpp"
#include "ZmqContext.hpp"
#include "nlohmann/json.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "work.hpp"
#include "zmq.hpp"
#include <cstdlib>
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
      checkerSocket_(::SERVICE_NAMESPACE::zmq::context(),
                     ::zmq::socket_type::push) {

  std::string checkerAddr = config["checker-addr"].As<std::string>();
  checkerSocket_.connect(checkerAddr);
}
using namespace userver;
yaml_config::Schema WorkHolder::GetStaticConfigSchema() {
  return yaml_config::MergeSchemas<components::ComponentBase>(R"(
type: object
description: send work to checker service
additionalProperties: false
properties:
  checker-addr:
    type: string
    description: address of checker service(tcp://checker:55555) for example
)");
}

void WorkHolder::addTask(const CheckerRequest &request) {
  using namespace userver::server::handlers;
  ::zmq::message_t msg{nlohmann::json(request).dump()};
  auto result = checkerSocket_.send(msg, ::zmq::send_flags::dontwait);
  if (!result.has_value())
    throw ExceptionWithCode<HandlerErrorCode::kTooManyRequests>{ExternalBody{
        "Checker service is overload now, please wait and try later"}};
}

} // namespace SERVICE_NAMESPACE
