#ifndef HANDLER_HELLO_HPP_
#define HANDLER_HELLO_HPP_

#include "userver/server/http/http_request.hpp"
#include <userver/server/handlers/http_handler_base.hpp>
namespace SERVICE_NAMESPACE {
using namespace userver;
class HelloHandler : public server::handlers::HttpHandlerBase {
public:
  constexpr static std::string_view kName = "hello-handler";
	using HttpHandlerBase::HttpHandlerBase;
	using HttpRequest = server::http::HttpRequest;
	using RequestContext = server::request::RequestContext;

	std::string HandleRequestThrow(const HttpRequest &request,
                               RequestContext &context) const override;
};
} // namespace SERVICE_NAMESPACE
#endif // !HANDLER_HELLO_HPP_
