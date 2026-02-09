#ifndef CUSTOM_TYPES_MAPPING_HPP_
#define CUSTOM_TYPES_MAPPING_HPP_
#include "userver/storages/postgres/io/pg_types.hpp"
#include "userver/utils/string_literal.hpp"
#pragma once

#include "userver/storages/postgres/io/io_fwd.hpp"
#include "userver/utils/trivial_map.hpp"
namespace SERVICE_NAMESPACE {
enum class UserRole { kStudent, kAdmin };
enum class WorkType { kLR, kKP };
enum class MarkType { k0, k2, k3, k4, k5 };
enum class WorkPassStatus { kNull, kChecking, kRejected, kAccepted };
} // namespace SERVICE_NAMESPACE

// This specialization MUST go to the header together with the mapped type
template <>
struct userver::storages::postgres::io::CppToUserPg<
    SERVICE_NAMESPACE::UserRole> {
  static constexpr DBTypeName postgres_name = "public.role";
  static constexpr USERVER_NAMESPACE::utils::TrivialBiMap enumerators =
      [](auto selector) {
        return selector()
            .Case("student", SERVICE_NAMESPACE::UserRole::kStudent)
            .Case("admin", SERVICE_NAMESPACE::UserRole::kAdmin);
      };
};

template <>
struct userver::storages::postgres::io::CppToUserPg<
    SERVICE_NAMESPACE::WorkType> {
  static constexpr DBTypeName postgres_name = "public.work_type_enum";
  static constexpr USERVER_NAMESPACE::utils::TrivialBiMap enumerators =
      [](auto selector) {
        return selector()
            .Case("LR", SERVICE_NAMESPACE::WorkType::kLR)
            .Case("KP", SERVICE_NAMESPACE::WorkType::kKP);
      };
};

template <>
struct userver::storages::postgres::io::CppToUserPg<
    SERVICE_NAMESPACE::MarkType> {
  static constexpr DBTypeName postgres_name = "public.mark_type";
  static constexpr USERVER_NAMESPACE::utils::TrivialBiMap enumerators =
      [](auto selector) {
        return selector()
            .Case("0", SERVICE_NAMESPACE::MarkType::k0)
            .Case("2", SERVICE_NAMESPACE::MarkType::k2)
            .Case("3", SERVICE_NAMESPACE::MarkType::k3)
            .Case("4", SERVICE_NAMESPACE::MarkType::k4)
            .Case("5", SERVICE_NAMESPACE::MarkType::k5);
      };
};

template <>
struct userver::storages::postgres::io::CppToUserPg<
    SERVICE_NAMESPACE::WorkPassStatus> {
  static constexpr DBTypeName postgres_name = "public.work_pass_status";
  static constexpr USERVER_NAMESPACE::utils::TrivialBiMap enumerators =
      [](auto selector) {
        return selector()
            .Case("null", SERVICE_NAMESPACE::WorkPassStatus::kNull)
            .Case("checking", SERVICE_NAMESPACE::WorkPassStatus::kChecking)
            .Case("rejected", SERVICE_NAMESPACE::WorkPassStatus::kRejected)
            .Case("accepted", SERVICE_NAMESPACE::WorkPassStatus::kAccepted);
      };
};

// short mappers
namespace SERVICE_NAMESPACE {
constexpr static auto &UserRoleMapper =
    ::userver::storages::postgres::io::CppToUserPg<UserRole>::enumerators;
constexpr static auto &WorkTypeMapper =
    ::userver::storages::postgres::io::CppToUserPg<WorkType>::enumerators;
constexpr static auto &MarkTypeMapper =
    ::userver::storages::postgres::io::CppToUserPg<MarkType>::enumerators;
constexpr static auto &WorkPassStatusMapper =
    ::userver::storages::postgres::io::CppToUserPg<WorkPassStatus>::enumerators;

template <typename T> std::string MapToString(const T &val) {
  constexpr static auto &bimap =
      ::userver::storages::postgres::io::CppToUserPg<T>::enumerators;
  auto literal = bimap.TryFindBySecond(val);
  return std::string(literal.value_or(userver::utils::StringLiteral("")));
}
} // namespace SERVICE_NAMESPACE
#endif // !CUSTOM_TYPES_MAPPING_HPP_
