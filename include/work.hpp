#ifndef WORK_HPP_
#define WORK_HPP_
#include <chrono>
#pragma once

namespace impl {
class IWork {
public:
	using time_point_t = std::chrono::time_point<std::chrono::steady_clock>; 
	virtual std::string to_string() const = 0;
	virtual time_point_t created_at() const = 0;
};
} // namespace impl

#endif // !WORK_HPP_
