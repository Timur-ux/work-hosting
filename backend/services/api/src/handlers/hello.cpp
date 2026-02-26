#include "handlers/hello.hpp"
#include "userver/http/content_type.hpp"
#include <fmt/core.h>
namespace SERVICE_NAMESPACE {
std::string HelloHandler::HandleRequestThrow(const HttpRequest &request,
                                                RequestContext &context) const {
	request.GetHttpResponse().SetContentType(http::content_type::kTextPlain);
	std::string username = context.GetData<std::string>("username");
	return fmt::format("Hello: {}", username);
}
}; // namespace SERVICE_NAMESPACE
