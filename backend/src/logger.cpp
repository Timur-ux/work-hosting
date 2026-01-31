#include "logger.hpp"
#include <ctime>
#include <iostream>
#include <mutex>
#include <ostream>

std::mutex TimedLog::mutex_{};

TimedLog::TimedLog(const char * format)
	: format_(format) {}

TimedLog::~TimedLog() {
	static char buffer[80];
	std::unique_lock lock(mutex_);

	std::string s = this->str();
	if(s.empty()) 
		return;

	std::time_t currentTime = std::time(nullptr);
	std::tm* timeinfo = std::localtime(&currentTime);
	std::strftime(buffer, sizeof(buffer), format_, timeinfo);

	std::cout << "[" << buffer << "] " << this->str() << '\n';
	std::flush(std::cout);
	this->str("");
}
