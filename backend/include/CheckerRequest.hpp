#ifndef CHECKER_REQUEST_HPP_
#define CHECKER_REQUEST_HPP_
#include "work.hpp"
#include <cstdint>
#pragma once
struct CheckerRequest {
	Work work;
	std::string first_name;
	std::string last_name;
	std::string father_name;
	std::int32_t group_number;
	std::int32_t in_group_order;
};

#endif // !CHECKER_REQUEST_HPP_
