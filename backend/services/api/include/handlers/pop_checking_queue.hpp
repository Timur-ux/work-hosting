#ifndef POP_CHECKING_QUEUE_HANDLER_HPP_
#define POP_CHECKING_QUEUE_HANDLER_HPP_
#include "db/schemas.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"
#include "userver/storages/postgres/transaction.hpp"
#include "work.hpp"
#include <cstdint>
#pragma once

#include <userver/server/handlers/http_handler_base.hpp>

namespace SERVICE_NAMESPACE {
using namespace userver;
class PopCheckingQueueHandler final : public server::handlers::HttpHandlerBase {
public:
  constexpr static std::string_view kName = "pop-checking-queue-handler";

  using HttpRequest = server::http::HttpRequest;
  using RequestContext = server::request::RequestContext;

  PopCheckingQueueHandler(
      const components::ComponentConfig &config,
      const components::ComponentContext &component_context);

  std::string HandleRequestThrow(const HttpRequest &request,
                                 RequestContext &context) const override;

private:
	using QueueEntryId = std::int64_t;
  void AcceptWork(const CheckingQueueItem &queuedWork, storages::postgres::Transaction &tr) const;
  void RejectWork(const CheckingQueueItem &queuedWork, storages::postgres::Transaction &tr) const;
  void RequeueWork(const CheckingQueueItem &queuedWork, storages::postgres::Transaction &tr) const;
  storages::postgres::ClusterPtr db_;
};
} // namespace SERVICE_NAMESPACE

#endif // !POP_CHECKING_QUEUE_HANDLER_HPP_
