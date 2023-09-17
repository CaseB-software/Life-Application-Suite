#pragma once

#include "OpenLog/OpenLog.h"
#include "Utilities/UTL_Files.h"
#include "Utilities/UTL_DateTime.h"
#include "Utilities/UTL_Text.h"

#include <iostream>
class LogManager : public OpenLog::LogTarget 
{
public:
	LogManager(const std::string name);
	~LogManager() = default;

	bool LogEvent(const OpenLog::Log& log) override;

	bool setParentDirectory(const std::string path);
	bool createLogFile();

private:
	std::filesystem::path m_logDirectory;
	std::filesystem::path m_instanceLogFile;

};

