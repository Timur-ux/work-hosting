#include "logger.hpp"
#include <ctime>
#include <mutex>

std::mutex TimedLog::mutex_{};

TimedLog::TimedLog(std::ostream & os, const char * format)
	: format_(format), os_(os) {}

TimedLog::~TimedLog() {
	flush();
}

void TimedLog::flush() {
	static char buffer[80];
	std::unique_lock lock(mutex_);

	std::string s = this->str();
	if(s.empty()) 
		return;

	std::time_t currentTime = std::time(nullptr);
	std::tm* timeinfo = std::localtime(&currentTime);
	std::strftime(buffer, sizeof(buffer), format_, timeinfo);

	os_ << "[" << buffer << "] " << this->str() << '\n';
	this->str("");
}
