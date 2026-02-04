#include "handlers/login.hpp"
#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "schemas/login.hpp"
#include "userver/engine/sleep.hpp"
#include "userver/http/content_type.hpp"
#include "userver/logging/log.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/io/bytea.hpp"
#include "userver/storages/postgres/io/io_fwd.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"
#include "work_hosting/sql_queries.hpp"
#include "utils.hpp"
#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace SERVICE_NAMESPACE {
LoginHandler::LoginHandler(const components::ComponentConfig &config,
                           const components::ComponentContext &context)
    : HttpHandlerJsonBase(config, context),
      db_(context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {
  const char *tokenTTL_str = getenv("TOKEN_TTL");
  if (!tokenTTL_str)
    throw std::runtime_error("TOKEN_TTL env not specified!");

  long tokenTTL = std::stoll(tokenTTL_str);
  if (tokenTTL <= 0)
    throw std::invalid_argument(
        "Given TOKEN_TTL is not positive, only positive TOKEN_TTL allowed");

  using namespace std::chrono;
  milliseconds tokenTTL_ms{tokenTTL};

  auto cleanupFunc = [db = db_, timeout_ms = tokenTTL_ms * 10,
                      token_ttl = tokenTTL_ms]() {
    while (true) {
      LOG_INFO() << "Start sleep for " << timeout_ms << "ms";
      engine::InterruptibleSleepFor(timeout_ms);
      LOG_INFO() << "Waked up from sleep, going to clear expired tokens";
      auto res = db->Execute(storages::postgres::ClusterHostType::kMaster,
                             sql::kDeleteExpiredTokens, token_ttl);
      LOG_INFO() << "Cleared " << res.RowsAffected() << " expired tokens";
    }
  };
  bts_.AsyncDetach("login-tokens-cleanup", cleanupFunc);
}

LoginHandler::Value
LoginHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                     const Value &request_json,
                                     RequestContext &context) const {
	request.GetHttpResponse().SetContentType(http::content_type::kApplicationJson);
  auto body = request_json.As<login::LoginRequestBody>();

	auto transaction = db_->Begin("login", storages::postgres::ClusterHostType::kMaster, {});
	auto res = transaction.Execute(sql::kFindUser, body.username);
	if(res.Size() == 0)
		throw server::handlers::ResourceNotFound(ExternalBody{"Login or password incorrect"});

	auto user = res.AsSingleRow<User>(storages::postgres::kRowTag);
	if(user.password_hash != utils::hash(body.password)) 
		throw server::handlers::ResourceNotFound(ExternalBody{"Login or password incorrect"});
	
	using RoleBimap = storages::postgres::io::CppToUserPg<UserRole>;
	using storages::postgres::Bytea;
	std::string token = utils::hash(body.username + utils::genSalt(25));
	res = transaction.Execute(sql::kInsertToken, Bytea(token), user.id, std::vector<std::string>{RoleBimap::enumerators.TryFindBySecond(user.role)->data()}, user.username);
	if(res.RowsAffected()) {
		transaction.Commit();
		return formats::json::ValueBuilder{login::LoginResponseBody{"success", token}}.ExtractValue();
	}

	throw server::handlers::InternalServerError{ExternalBody{"Can't insert generated auth token to db"}};
}

LoginHandler::~LoginHandler() { bts_.CancelAndWait(); }
} // namespace SERVICE_NAMESPACE
 
