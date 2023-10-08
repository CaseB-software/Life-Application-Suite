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

	//std::vector<std::unique_ptr<OpenLog::Log>> m_storedLogs;		// Logs stored here need to be dumped to log file. If dumped, they are erased
};

