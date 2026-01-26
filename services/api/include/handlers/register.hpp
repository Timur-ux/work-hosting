#ifndef HANDLER_REGISTER_HPP_
#define HANDLER_REGISTER_HPP_
#pragma once
#include <userver/server/handlers/http_handler_json_base.hpp>
namespace SERVICE_NAMESPACE {
class RegisterHandler : public userver::server::handlers::HttpHandlerJsonBase {
public:
	using HttpHandlerJsonBase::HttpHandlerJsonBase;
	constexpr static std::string_view kName = "register-handler";

	Value HandleRequestJsonThrow(const HttpRequest& request, const Value& request_json, RequestContext& context) const;
};
} // namespace SERVICE_NAMESPACE
#endif // !HANDLER_REGISTER_HPP_
