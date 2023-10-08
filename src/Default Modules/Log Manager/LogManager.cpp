#include "LogManager.h"

LogManager::LogManager(const std::string name) : OpenLog::LogTarget{ name } { 

}
bool LogManager::LogEvent(const OpenLog::Log& log) {

	if (utl::fs::doesFileExist(m_logDirectory.string())) {
		std::ofstream file;

		file.open(m_instanceLogFile, std::ios_base::app);

		// Dump stored logs to file and erase the element
		/*
		if (m_storedLogs.size() != 0) {
			file.open(m_instanceLogFile, std::ios_base::app);
			for (std::vector <std::unique_ptr<OpenLog::Log>>::iterator iter{m_storedLogs.begin()}; iter != m_storedLogs.end();) {
				file << OpenLog::printLog(*iter->get());
				iter->release();
				iter = m_storedLogs.erase(iter);
			}
			file.close();
		}
		*/
		file << OpenLog::printLog(log);
		file.close();

		if (file.fail()) {
			std::cerr << "std::ofstream failbit flag raised. Could not write to file [" + m_instanceLogFile.string() + ']' << std::endl;
			return false;
		}
		return true;
	}
	else {
		//m_storedLogs.push_back(std::make_unique<OpenLog::Log>(log));
		return true;
	}
	return false;
}

bool LogManager::setParentDirectory(const std::string path) {
	if (utl::fs::doesFileExist(path)) {
		m_logDirectory = path;
		return true;
	}
	else
		return false;
}
bool LogManager::createLogFile() {
	utl::time::Timestamp now{};
	now.stamp();

	if (m_logDirectory.string() == "") {
		return false;
	}
	else {
		std::string initialPath{ m_logDirectory.string() };
		utl::txt::ensureBackslash(initialPath);

		std::ostringstream fullPath;
		fullPath << initialPath << now.printNumericDateAndTimeNoSpaces() << ".log";

		if (utl::fs::createFile(fullPath.str())) {
			m_instanceLogFile = fullPath.str();
			return true;
		}
		else
			return false;
	}
	

	return false;
}
