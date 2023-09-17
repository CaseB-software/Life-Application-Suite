#pragma once

#include "UTL_Settings.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <exception>

#include <iostream>

namespace utl{

	namespace fs {

		bool createDirectory	(const std::string path);									// Return TRUE if directory was found/created. FALSE if directory could not be created
		bool createFile			(const std::string path);									// Return TRUE if file was found/created. FALSE if file could not be created
		bool deleteFile			(const std::string path);									// Return TRUE if file could be deleted, FALSE is file could not be deleted
		bool renameFile			(const std::string oldPath, const std::string newPath);		// Return TRUE if target file could be found and renamed, FALSE if target file could not be found
		bool doesFileExist		(const std::string path);									// Return TRUE if file exists. FALSE if it does not exist


		std::ofstream getFileToAppend(const std::string path);								// Opens returns stream to file in append mode
		bool overwriteFile		(const std::string path, const std::string& text);			// Writes the given text to the file. Overwrites everything in the file. Returns TRUE if file was written, FALSE if file could not be found
		bool readFile			(const std::string path, std::ostringstream& output);		// Reads all text from a file and overwrites parameter output with the text. Returns TRUE if file was opened and read from, FALSE if file could not be found

		std::vector<std::string> getFilesInDirectory(const std::string directoryToRead);	// Iterates through the given directory and adds file name to the return vector

	}
}


