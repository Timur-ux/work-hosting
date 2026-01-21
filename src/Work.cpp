#include "work.hpp"
#include <format>
#include <stdexcept>

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
