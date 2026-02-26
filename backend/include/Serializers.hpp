#ifndef SERIALIZERS_HPP_
#define SERIALIZERS_HPP_
#include "CheckerRequest.hpp"
#include "work.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#pragma once
#include "nlohmann/json.hpp"

using json = nlohmann::json;
static void to_json(json &j, const Work &work) {
  j = json{{"type", work.typeAsString()},
           {"gv-name", work.gvName()},
           {"number", work.number()}};
}

static void from_json(const json &j, Work &work) {
  std::string typeStr;
  unsigned short number;
  std::string gvName;

  j.at("type").get_to(typeStr);
  Work::Type type = (typeStr == "LR" ? Work::Type::LR : Work::Type::KP);

  j.at("gv-name").get_to(gvName);
  j.at("number").get_to(number);
  work = Work(type, number, gvName);
}

static void to_json(json &j, const CheckerRequest &request) {
  j = json{{"work", json(request.work)},
           {"first-name", request.first_name},
           {"last-name", request.last_name},
           {"father-name", request.father_name},
           {"group-number", request.group_number},
           {"in-group-order", request.in_group_order}};
}

static void from_json(const json &j, CheckerRequest &request) {
  j.at("work").get_to(request.work);
  j.at("first-name").get_to(request.first_name);
  j.at("last-name").get_to(request.last_name);
  j.at("father-name").get_to(request.father_name);
  j.at("group-number").get_to(request.group_number);
  j.at("in-group-order").get_to(request.in_group_order);
}
#endif // !SERIALIZERS_HPP_
