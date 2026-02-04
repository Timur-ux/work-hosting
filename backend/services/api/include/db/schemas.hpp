#ifndef DB_SCHEMAS_HPP_
#define DB_SCHEMAS_HPP_
#include "userver/storages/postgres/io/bytea.hpp"
#include "userver/storages/postgres/io/chrono.hpp"
#include "db/CustomTypesMapping.hpp"
#include <cstdint>
#include <string>
#include <vector>
#pragma once
namespace SERVICE_NAMESPACE {
struct User {
  std::int64_t id;
  std::string username;
	std::string password_hash;
	UserRole role;
  userver::storages::postgres::TimePointWithoutTz created_at;
};

struct Student {
  std::int64_t id;
  std::int64_t user_id;
  std::string gv_name;
  std::string first_name;
  std::string last_name;
  std::string father_name;
  std::string initials;
  std::string email;
  userver::storages::postgres::TimePointWithoutTz updated_at;
};

} // namespace SERVICE_NAMESPACE
#endif // !DB_SCHEMAS_HPP_
