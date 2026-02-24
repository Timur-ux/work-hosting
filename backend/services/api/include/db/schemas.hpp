#ifndef DB_SCHEMAS_HPP_
#define DB_SCHEMAS_HPP_
#include "db/CustomTypesMapping.hpp"
#include "userver/storages/postgres/io/chrono.hpp"
#include <cstdint>
#include <string>
#include <vector>
#pragma once
namespace SERVICE_NAMESPACE {
struct UserBase {
  std::int64_t id;
  std::string first_name;
  std::string last_name;
  std::int32_t group_number;
  std::int32_t in_group_order;
};

struct User {
  std::int64_t id;
  std::string username;
  std::string password_hash;
  UserRole role;
  userver::storages::postgres::TimePointTz created_at;
};

struct Student {
  std::int64_t id;
  std::int64_t user_id;
  std::string gv_name;
  std::int32_t group_number;
  std::int32_t in_group_order;
  std::string first_name;
  std::string last_name;
  std::string father_name;
  std::string email;
  userver::storages::postgres::TimePointTz updated_at;
};

struct Work {
  std::int64_t id;
  std::int32_t work_number;
  std::int32_t old_work_number;
  WorkType work_type;
  std::string theme;
  userver::storages::postgres::TimePointTz created_at;
};

struct CheckingQueueItem {
	std::int64_t queue_id;
  std::string gv_name;
  WorkType work_type;
  std::int32_t old_work_number;
};

struct DenormalizedMark {
  std::int64_t user_id;
  MarkType mark;
  WorkPassStatus status;
  WorkType work_type;
  std::int32_t old_work_number;
};

} // namespace SERVICE_NAMESPACE
#endif // !DB_SCHEMAS_HPP_
