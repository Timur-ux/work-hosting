#include "handlers/login.hpp"
#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "schemas/auth.hpp"
#include "schemas/login.hpp"
#include "userver/http/content_type.hpp"
#include "userver/logging/log.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/server/handlers/http_handler_json_base.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/exceptions.hpp"
#include "userver/storages/postgres/options.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"
#include "userver/storages/postgres/transaction.hpp"
#include "userver/storages/redis/client.hpp"
#include "userver/storages/redis/component.hpp"
#include "userver/yaml_config/merge_schemas.hpp"
#include "utils.hpp"
#include "work_hosting/sql_queries.hpp"
#include <chrono>
#include <cstdlib>
#include <string>

namespace SERVICE_NAMESPACE {
LoginHandler::LoginHandler(const components::ComponentConfig &config,
                           const components::ComponentContext &context)
    : HttpHandlerJsonBase(config, context),
      db_(context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()),
      redis_(context.FindComponent<components::Redis>("redis-cache")
                 .GetClient("api_cache_db")) {
  long tokenTTL = config["token-ttl"].As<long>();
  if (tokenTTL <= 0) {
    LOG_WARNING() << "Given TOKEN_TTL is not positive: " << tokenTTL
                  << "\nIt must be positive, so i use default value: "
                  << tokenTTL_ms_ << "ms";
    return;
  }
  tokenTTL_ms_ = tokenTTL;
  LOG_INFO() << "Using token ttl: " << tokenTTL_ms_ << "ms";
}

yaml_config::Schema LoginHandler::GetStaticConfigSchema() {
	return yaml_config::MergeSchemas<HttpHandlerJsonBase>(R"(
type: object
additionalProperties: false
description: provide auto token ttl
properties:
  token-ttl:
    type: integer
    description: auth token time to live in milliseconds
)");
}

LoginHandler::Value
LoginHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                     const Value &request_json,
                                     RequestContext &context) const {
  request.GetHttpResponse().SetContentType(
      http::content_type::kApplicationJson);
  auto body = request_json.As<login::LoginRequestBody>();

	using storages::postgres::TransactionOptions;
	TransactionOptions opts{TransactionOptions::kReadOnly};
  auto transaction =
      db_->Begin("login", storages::postgres::ClusterHostType::kSlave, opts);

  auto res = transaction.Execute(sql::kFindUser, body.username);
  if (res.Size() == 0)
    throw server::handlers::ResourceNotFound(
        ExternalBody{"Login or password incorrect"});

  auto user = res.AsSingleRow<User>(storages::postgres::kRowTag);
  if (user.password_hash != utils::hash(body.password))
    throw server::handlers::ResourceNotFound(
        ExternalBody{"Login or password incorrect"});

  using RoleBimap = storages::postgres::io::CppToUserPg<UserRole>;
  std::string token = utils::hash(body.username + utils::genSalt(25));
  transaction.Commit();
  auth::AuthCacheEntry cacheEntry{
      user.id,
      user.username,
      std::vector<std::string>{
          RoleBimap::enumerators.TryFindBySecond(user.role)->data()},
  };

  auto cacheEntryValue = formats::json::ValueBuilder{cacheEntry}.ExtractValue();

  using std::chrono::milliseconds;
	try{
		redis_
				->Set(token, formats::json::ToString(cacheEntryValue),
							milliseconds(tokenTTL_ms_), redisCC_)
				.Get();
	} catch(std::exception &e) {
		LOG_ERROR() << "Set auth token to redis failed: " << e.what();
	}
  return formats::json::ValueBuilder{login::LoginResponseBody{"success", token}}
      .ExtractValue();
  return formats::json::ValueBuilder{login::LoginResponseBody{"success", "some"}}
      .ExtractValue();
}

} // namespace SERVICE_NAMESPACE
