#pragma once

#include "UTL_Settings.h"

#include <string>
#include <sstream>
#include <algorithm>


namespace utl {
	namespace txt {
		// Reads text until limit and deletes characters read \param text: what will be read, limit: char to read until, returnType: will be overwritten with the data
		template <typename T>
		void readUntilAuto(std::string& text, const char limit, T& returnType) {
			std::stringstream bufferText;

			int charRead{ 0 };
			for (char& c : text) {
				++charRead;
				if (c != limit) {
					bufferText << c;
				}
				else {
					bufferText >> returnType;
					text.erase(0, charRead);
					return;
				}
			}
		}

		std::string readUntilString(std::string& text, const char limit);	// Reads text until limit char found and deletes characters read	
		
		void ensureBackslash(std::string& input);	// Ensure there is a backslash for the given string. If not found, it emplaces '\\'
	
		std::string toUpper(std::string text);		// Converts string to all upper case
		std::string toLower(std::string text);		// Converts string to all lower case


	}
}