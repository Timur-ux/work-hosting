#ifndef HANDLER_SEND_WORK_HPP_
#define HANDLER_SEND_WORK_HPP_
#include "components/workHolder.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"
#include "userver/storages/redis/client_fwd.hpp"
#include "userver/storages/redis/command_control.hpp"
#include "userver/yaml_config/schema.hpp"
#pragma once
#include <userver/server/handlers/http_handler_json_base.hpp>
namespace SERVICE_NAMESPACE {
class SendWorkHandler : public userver::server::handlers::HttpHandlerJsonBase {
public:
	constexpr static std::string_view kName = "send-work-handler";

	SendWorkHandler(const userver::components::ComponentConfig & config, const userver::components::ComponentContext & context);

	Value HandleRequestJsonThrow(const HttpRequest& request, const Value& request_json, RequestContext& context) const;
	static userver::yaml_config::Schema GetStaticConfigSchema();
private:
	long send_timeout_ms_ = 60000;
	WorkHolder & workHolder_;
	userver::storages::postgres::ClusterPtr db_;
	userver::storages::redis::ClientPtr redis_;
	userver::storages::redis::CommandControl redisCC_;
};
} // namespace SERVICE_NAMESPACE
#endif // !HANDLER_SEND_WORK_HPP_

