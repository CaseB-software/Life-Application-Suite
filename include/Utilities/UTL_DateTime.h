#pragma once
#include <chrono>
#include <format>

#include "UTL_Settings.h"
#include "UTL_json.h"

namespace utl {

	namespace time {

		class Timestamp {
		public:
			Timestamp();
			Timestamp(std::chrono::system_clock::time_point time);
			virtual ~Timestamp();


			virtual bool stamp(const std::chrono::system_clock::time_point& time = std::chrono::system_clock::now());						// Stamps the time as now if m_isStamped is false
			
			virtual std::string printDay()		const;						// Prints DD (09)
			virtual std::string printMonth()	const;						// Prints MM (01 for January)

			virtual std::string printDate()		const;						// Prints in DD MMM YYYY format (12 Dec 2022)
			virtual std::string printHMS()		const;						// Prints the HH:MM::SS			(14:23:01)
			virtual std::string print()			const;						// Is the same as outputting getRawTimme to ostream
			virtual std::string printNumericDateAndTimeNoSpaces() const;	// Prints format DD-MM-YYYY_HH-MM-SS
	
			bool isStamped() const;

			std::chrono::system_clock::time_point	getRawTime()	const;	// Returns m_time
			std::chrono::year_month_day				getRawYMD()		const;	// Returns m_time as a YMD

			friend std::ostream& operator<<(std::ostream& os, const Timestamp& time);

		private:
			std::chrono::system_clock::time_point m_time{};
			bool m_isStamped{ false };
		};

	
		class Timer
		{
		public:
			Timer();
			Timer(const std::chrono::system_clock::time_point& start);
			virtual ~Timer();

			virtual bool start	(const std::chrono::system_clock::time_point& time = std::chrono::system_clock::now());
			virtual bool end	(const std::chrono::system_clock::time_point& time = std::chrono::system_clock::now());

			bool isRunning()	const;
			bool isFinished()	const;

			const Timestamp& getStartTimestamp()	const;
			const Timestamp& getEndTimestamp()		const;

			std::chrono::system_clock::duration		getRawDuration()	const;
			std::chrono::system_clock::time_point	getRawStartTime()	const;
			std::chrono::system_clock::time_point	getRawEndTime()		const;

			std::string printDate()			const;	// Returns the Timestamp printDate() of m_start
			std::string printDuration()		const;	// Returns the total time if ended
			std::string printStartTime()	const;	// Returns the m_start printHMS()
			std::string printEndTime()		const;	// Returns the m_end printHMS()

		private:
			Timestamp	m_start{}, m_end{};
			std::chrono::system_clock::duration		m_rawDuration{};

			bool m_running	{ false };
			bool m_finished	{ false };
		};


		class LabeledTimer : public Timer {
		public:
			LabeledTimer(const std::string& name = "New Timer");
			~LabeledTimer();

			std::string getName() const;

			void setName(std::string name);

		private:
			std::string m_name{};
		};




		std::chrono::system_clock::time_point stringToTimepoint(const std::string& time); // Converts a string of "YYYY-MM-DD HH:MM::SS" into a timepoint

		void to_json	(utl::json::json& j, const Timestamp& t);
		void from_json	(const utl::json::json& j, Timestamp& t);

		void to_json	(utl::json::json& j, const Timer& t);
		void from_json	(const utl::json::json& j, Timer& t);

		void to_json	(utl::json::json& j, const LabeledTimer& t);
		void from_json	(const utl::json::json& j, LabeledTimer& t);
	}
}