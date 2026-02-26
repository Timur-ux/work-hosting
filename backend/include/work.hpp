#ifndef WORK_HPP_
#define WORK_HPP_
#include <string>
#include <string_view>
#pragma once

#ifndef MAX_WORK_NUMBER
#define MAX_WORK_NUMBER 50
#endif // !MAX_WORK_NUMBER

class Work {
public:
  enum class Type { LR, KP };

	Work() = default;
	Work(Type type, unsigned short number, const std::string &gvName);

  Type type() const;
	const char * typeAsString() const;
	const std::string &gvName() const;
	unsigned short number() const;
	const std::string & to_string() const; // <work type><work number>-<gvName> 
	static Work from_string(std::string_view);
private:
	std::string to_string_() const;

  Type type_;
  unsigned short number_;
	std::string gvName_;
	std::string stringView_;
};
#endif // !WORK_HPP_
