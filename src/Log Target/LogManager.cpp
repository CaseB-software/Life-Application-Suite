#include "LogManager.h"

LogManager::LogManager(const std::string name) : OpenLog::LogTarget{ name } { 

}
bool LogManager::LogEvent(const OpenLog::Log& log) {
	std::cout << OpenLog::defaultFormatLog(log);

	if (FileSystem::doesFileExist(m_logDirectory.string())) {
		std::ofstream file;

		file.open(m_instanceLogFile, std::ios_base::app);
		file << OpenLog::defaultFormatLog(log);
		file.close();

		if (file.fail()) {
			std::cerr << "std::ofstream failbit flag raised. Could not write to file [" + m_instanceLogFile.string() + ']' << std::endl;
			return false;
		}
		return true;
	}
	else
		return false;
}

bool LogManager::setParentDirectory(const std::string path) {
	if (FileSystem::doesFileExist(path)) {
		m_logDirectory = path;
		return true;
	}
	else
		return false;
}
bool LogManager::createLogFile() {
	Timestamp now{};
	now.stamp();

	if (m_logDirectory.string() == "") {
		return false;
	}
	else {
		std::string initialPath{ m_logDirectory.string() };
		Utilities::ensureBackslash(initialPath);

		std::ostringstream fullPath;
		fullPath << initialPath << now.printNumericDateAndTimeNoSpaces() << ".log";

		if (FileSystem::createFile(fullPath.str())) {
			m_instanceLogFile = fullPath.str();
			return true;
		}
		else
			return false;
	}
	

	return false;
}
