#include "handlers/register.hpp"
#include "db/CustomTypesMapping.hpp"
#include "db/schemas.hpp"
#include "schemas/registration.hpp"
#include "userver/components/component_context.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/http/content_type.hpp"
#include "userver/logging/log.hpp"
#include "userver/server/handlers/exceptions.hpp"
#include "userver/server/handlers/fallback_handlers.hpp"
#include "userver/server/http/http_status.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/io/bytea.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "utils.hpp"
#include "work_hosting/sql_queries.hpp"
#include <cryptopp/sha.h>
#include <cstdlib>
#include <fmt/compile.h>
#include <fmt/core.h>
#include <stdexcept>
#include <userver/formats/serialize/common_containers.hpp>

namespace SERVICE_NAMESPACE {
RegisterHandler::RegisterHandler(
    const components::ComponentConfig &config,
    const components::ComponentContext &component_context)
    : HttpHandlerJsonBase(config, component_context),
      db_(component_context.FindComponent<components::Postgres>("pg-database")
              .GetCluster()) {
  const char *adminUsername = std::getenv("ADMIN_USERNAME");
  if (!adminUsername)
    throw std::runtime_error("ADMIN_USERNAME env not specified!");
  const char *adminPassword = std::getenv("ADMIN_PASSWORD");
  if (!adminPassword)
    throw std::runtime_error("ADMIN_PASSWORD env not specified!");

  std::string passwordHash = utils::hash(adminPassword);

  auto transaction = db_->Begin(
      "check-admin", storages::postgres::ClusterHostType::kMaster, {});
  auto res = transaction.Execute(sql::kFindUser, adminUsername);
  if (res.Size() == 0) {
		using storages::postgres::Bytea; 
    LOG_INFO() << "Admin user not found so i insert new";
    res = transaction.Execute(sql::kInsertUser, adminUsername, Bytea(passwordHash),
                              UserRole::kAdmin);
    if (res.RowsAffected()) {
      transaction.Commit();
      LOG_INFO() << "Admin with username: " << adminUsername
                 << " successfully inserted";
      return;
    }
    LOG_ERROR() << "Admin with username: " << adminUsername
                << " can't be inserted to db!";
    return;
  }
  auto user = res.AsSingleRow<User>(storages::postgres::kRowTag);
  if (user.password_hash == passwordHash && user.role == UserRole::kAdmin) {
    LOG_INFO() << "Same admin user already exists in db";
    return;
  }

  res = transaction.Execute(sql::kUpdateAdmin, passwordHash, user.id);
  if (res.RowsAffected()) {
    transaction.Commit();
    LOG_INFO() << "Admin with username: " << adminUsername << " and id: " << user.id
               << " successfully updated to new password";
    return;
  }
  LOG_ERROR() << "Admin with username: " << adminUsername << " and id: " << user.id
              << " can't be updated to new password!";
  return;
}

RegisterHandler::Value
RegisterHandler::HandleRequestJsonThrow(const HttpRequest &request,
                                        const Value &request_json,
                                        RequestContext &context) const {
  request.GetHttpResponse().SetContentType(
      http::content_type::kApplicationJson);
  auto body = request_json.As<registration::RegisterRequestBody>();
  auto transaction =
      db_->Begin("register", storages::postgres::ClusterHostType::kMaster, {});

  auto res = transaction.Execute(sql::kIsUserExists, body.gv_name);
  if (res.AsSingleRow<bool>())
    throw server::handlers::ConflictError(
        ExternalBody{"Student with given git verse name already exists"});
  std::string passwordHash = utils::hash(body.password);
	using storages::postgres::Bytea;
  res = transaction.Execute(sql::kInsertUser, body.gv_name, Bytea(passwordHash),
                            UserRole::kStudent);
  if (!res.RowsAffected()) {
    LOG_WARNING() << "Unable insert user with username:" << body.gv_name;
    throw server::handlers::InternalServerError(
        ExternalBody{"Unable insert user to db"});
  }
  res = transaction.Execute(sql::kFindUser, body.gv_name);

  if (res.Size() != 1) {
    LOG_WARNING() << "After inserting user with username: " << body.gv_name
                  << " found: " << res.Size()
                  << " users with such username, expected 1";
    throw server::handlers::InternalServerError(
        ExternalBody{"Unable find proper user in db"});
  }

  auto user = res.AsSingleRow<User>(storages::postgres::kRowTag);

  std::string initials =
      fmt::format("{}{}", body.first_name[0], body.father_name[0]);

  res = transaction.Execute(sql::kInsertStudent, user.id, body.gv_name,
                            body.group, body.first_name, body.last_name,
                            body.father_name, initials, body.email);
  if (res.RowsAffected()) {
    transaction.Commit();
    request.SetResponseStatus(server::http::HttpStatus::kCreated);
    return formats::json::ValueBuilder{
        registration::RegisterResponseBody{
            "success",
            fmt::format("Registered user with username: {}", body.gv_name)}}
        .ExtractValue();
  }

  transaction.Rollback();
  throw server::handlers::InternalServerError{
      ExternalBody{"User can't be inserted to db -\\_(-_-)/-"}};
}

} // namespace SERVICE_NAMESPACE
