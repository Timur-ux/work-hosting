#include "work.hpp"
#include <exception>
#include <format>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

Work::Work(Type type, unsigned short number, const std::string &gvName)
	: type_(type), number_(number), gvName_(gvName) {
	if(number > MAX_WORK_NUMBER) 
		throw std::invalid_argument("Work number greater than MAX_WORK_NUMBER");
	stringView_ = to_string_();
}

Work::Type Work::type() const {
	return type_;
}
const char * Work::typeAsString() const {
	return type() == Work::Type::LR ? "LR" : "KP";
}

const std::string &Work::gvName() const {
	return gvName_;
}

unsigned short Work::number() const {
	return number_;
}

std::string Work::to_string_() const {
	return std::format("{}{}-{}", typeAsString(), number_, gvName_);
} 
const std::string & Work::to_string() const {
	return stringView_;
} 

Work Work::from_string(std::string_view s) {
	Type type;
	if(s.starts_with("LR")) 
		type = Type::LR;
	else if (s.starts_with("KP") == 0)
	 type = Type::KP;
	else
		throw std::invalid_argument(std::format("Work type is invalid! Expected LR or KP but given: {}", s.substr(0, 2)));
	size_t defis = s.find("-");
	if(defis == std::string::npos) 
		throw std::invalid_argument("Expected '-' between work number and username but '-' not found!");
	if(defis <= 2) 
		throw std::invalid_argument("Expected work number after work type but it is elapsed!");
	long workNumber;
	try {
		workNumber = std::stol(std::string(s.substr(2, defis - 2)), nullptr, 10);
	} catch (...) {
		throw std::invalid_argument("Error while converting work number from string to integer");
	}
	if(!(workNumber > 0 && workNumber < std::numeric_limits<unsigned short>::max())) 
		throw std::invalid_argument("Work number must be in unsigned short range");
	
	std::string_view gvName = s.substr(defis+1);

	return Work{type, static_cast<unsigned short>(workNumber), std::string(gvName)};
} 
