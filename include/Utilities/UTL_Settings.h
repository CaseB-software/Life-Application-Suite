#pragma once
#include <sstream>
#include <chrono>

namespace utl {

	std::string version();
	std::string compiledDate();
	
	void setHourOffset(int offset);			// Set the hour offset for timing functionality.
	std::chrono::hours	getHourOffset();

}