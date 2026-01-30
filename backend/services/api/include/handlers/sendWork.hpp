#ifndef HANDLER_SEND_WORK_HPP_
#define HANDLER_SEND_WORK_HPP_
#include "components/workHolder.hpp"
#pragma once
#include <userver/server/handlers/http_handler_json_base.hpp>
namespace SERVICE_NAMESPACE {
class SendWorkHandler : public userver::server::handlers::HttpHandlerJsonBase {
public:
	constexpr static std::string_view kName = "send-work-handler";

	SendWorkHandler(const userver::components::ComponentConfig & config, const userver::components::ComponentContext & context);

	Value HandleRequestJsonThrow(const HttpRequest& request, const Value& request_json, RequestContext& context) const;
private:
	WorkHolder & workHolder_;
};
} // namespace SERVICE_NAMESPACE
#endif // !HANDLER_SEND_WORK_HPP_

