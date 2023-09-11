#pragma once
#include <chrono>
#include <format>

class Timestamp {
public:
	Timestamp() {};
	Timestamp(std::chrono::system_clock::time_point time) : m_time{ time } { m_isStamped = true; }
	virtual ~Timestamp() {};

	virtual bool stamp(const std::chrono::system_clock::time_point& time = std::chrono::system_clock::now());						// Stamps the time as now if m_isStamped is false
	virtual std::string printDay()		const;
	virtual std::string printMonth()	const;

	virtual std::string printDate()		const;		// Prints in DD MMM YYYY format (12 Dec 2022)
	virtual std::string printTime()		const;		// Prints the HH:MM::SS			(14:23:01)
	virtual std::string print()			const;		// Is the same as outputting getRawTimme to ostream
	virtual std::string printNumericDateAndTimeNoSpaces() const;		// Prints format DD-MM-YY_HH:MM:SS


	bool isStamped()				const { return m_isStamped; }

	std::chrono::system_clock::time_point	getRawTime()	const { return m_time; };
	std::chrono::year_month_day				getRawYMD()		const;

private:
	std::chrono::system_clock::time_point m_time{};
	bool m_isStamped{ false };
};



class Timer 
{
public:
	Timer() {};
	Timer(const std::chrono::system_clock::time_point& start);
	virtual ~Timer() {};

	virtual bool start	(const std::chrono::system_clock::time_point& time = std::chrono::system_clock::now());
	virtual bool end	(const std::chrono::system_clock::time_point& time = std::chrono::system_clock::now());

	bool isRunning()	const;
	bool isFinished()	const;

	std::chrono::system_clock::duration		getRawDuration()	const { return m_rawDuration; }
	std::chrono::system_clock::time_point	getRawStartTime()	const { return m_start.getRawTime(); }
	std::chrono::system_clock::time_point	getRawEndTime()		const { return m_end.getRawTime(); }

	std::string printDate()			const { return m_start.printDate(); }
	std::string printDuration()		const;
	std::string printStartTime()	const;
	std::string printEndTime()		const;

private:
	Timestamp	m_start{}, m_end{};
	std::chrono::system_clock::duration		m_rawDuration{};

	bool m_running	{ false };
	bool m_finished	{ false };
};



class TimeEntry : public Timer {
public:
	TimeEntry(const std::string& name = "New Timer") : m_name{ name } { }
	~TimeEntry() {};

	std::string getName() const { return m_name; }
	
	void setName(std::string name) { m_name = name; }

private:
	std::string m_name{};
};


namespace DateTimeHelper {
	std::chrono::system_clock::time_point stringToTimepoint (const std::string& time);
}
