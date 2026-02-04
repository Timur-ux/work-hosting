#ifndef CUSTOM_TYPES_MAPPING_HPP_
#define CUSTOM_TYPES_MAPPING_HPP_
#include "userver/storages/postgres/io/pg_types.hpp"
#pragma once

#include "userver/storages/postgres/io/io_fwd.hpp"
#include "userver/utils/trivial_map.hpp"
namespace SERVICE_NAMESPACE {
enum class UserRole { kStudent, kAdmin };
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
#endif // !CUSTOM_TYPES_MAPPING_HPP_
