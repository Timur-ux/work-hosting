#include "handlers/sendWork.hpp"
#include "components/workHolder.hpp"
#include "schemas/check.hpp"
#include "userver/http/content_type.hpp"
#include "work.hpp"
#include <userver/components/component_context.hpp>
#include <userver/formats/json/serialize_container.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/handlers/exceptions.hpp>

namespace impl {
Work workMap(const check::WorkRequest &requestBody) {
  Work::Type type =
      requestBody.workType == "LR" ? Work::Type::LR : Work::Type::KP;
  if (!requestBody.workNumber.has_value())
    throw userver::server::handlers::ResourceNotFound(userver::server::handlers::ExternalBody{"Work number not set!"});
  if (!requestBody.gvName.has_value())
    throw userver::server::handlers::ResourceNotFound(userver::server::handlers::ExternalBody{"Gitverse name not set!"});

  return Work(type, static_cast<unsigned short>(*requestBody.workNumber),
              *requestBody.gvName);
}
} // namespace impl

namespace SERVICE_NAMESPACE {
SendWorkHandler::SendWorkHandler(
    const userver::components::ComponentConfig &config,
    const userver::components::ComponentContext &context)
    : HttpHandlerJsonBase(config, context),
      workHolder_(context.FindComponent<WorkHolder>()) {}

SendWorkHandler::Value
SendWorkHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                        const Value &request_json,
                                        RequestContext &context) const {
	request.GetHttpResponse().SetContentType(userver::http::content_type::kApplicationJson);
  auto requestBody = request_json.As<check::WorkRequest>();
  Work work = impl::workMap(requestBody);

  workHolder_.addTask(work);
  return userver::formats::json::ValueBuilder{
      check::SendWorkResponse{"Ok, added to check queue"}}
      .ExtractValue();
}

} // namespace SERVICE_NAMESPACE
