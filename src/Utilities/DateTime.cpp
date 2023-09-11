#include "DateTime.h"

std::string Timestamp::printDay() const {
	return std::format("{:%d}", m_time);
}
std::string Timestamp::printMonth() const {
	return std::format("{:%B}", m_time);
}
std::chrono::year_month_day	Timestamp::getRawYMD() const {
	std::chrono::year_month_day ymd { std::chrono::floor<std::chrono::days>(m_time) };
	return ymd;
}



std::string Timestamp::printDate() const {
	return std::format("{:%Od %b %Y}", m_time);
}
std::string Timestamp::printTime() const {
	return std::format("{:%H:%M:%OS}", m_time);
}
std::string Timestamp::print() const {
	std::ostringstream os;
	os << m_time;
	return os.str();
}
std::string Timestamp::printNumericDateAndTimeNoSpaces() const {
	return std::format("{:%Od-%m-%Y_%H-%M-%OS}", m_time);
}


bool Timestamp::stamp(const std::chrono::system_clock::time_point& time) {
	using namespace std::literals;

	if (!m_isStamped) {
		m_time = time - 7h; // 7 hour time offset for real time
		m_isStamped = true;
		return true;
	}
	else
		return false;
}





Timer::Timer(const std::chrono::system_clock::time_point& start) {
	this->start(start);
}

bool Timer::start(const std::chrono::system_clock::time_point& time) {

	if (!m_finished && !m_running) {
		m_start.stamp(time);
		m_running = true;
		return true;
	}
	else {
		return false;
	}

	return false;
}

bool Timer::end(const std::chrono::system_clock::time_point& end) {

	if (m_running && !m_finished) {
		m_end.stamp(end);
		m_rawDuration = m_end.getRawTime() - m_start.getRawTime();
		m_finished = true;
		m_running = false;
		return true;
	}
	else
		return false;

	return false;
}

bool Timer::isRunning() const{
	return m_running;
}
bool Timer::isFinished() const {
	return m_finished;
}

std::string Timer::printDuration() const {
	return std::format("{:%H:%M:%OS}", m_rawDuration);
}

std::string Timer::printStartTime() const {
	using namespace std::literals;

	return std::format("{:%OH:%OM:%OS}", this->getRawStartTime());
}
std::string Timer::printEndTime() const {
	using namespace std::literals;

	return std::format("{:%OH:%OM:%OS}", this->getRawEndTime());
}





std::chrono::system_clock::time_point DateTimeHelper::stringToTimepoint(const std::string& time) {
	std::stringstream ss{ time };
	std::tm _tm{};
	ss >> std::get_time(&_tm, "%Y-%m-%d %H:%M:%OS");
	return std::chrono::system_clock::from_time_t(std::mktime(&_tm));
}
