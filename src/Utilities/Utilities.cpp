#include "Utilities.h"

namespace Utilities {
	void ensureBackslash(std::string& input) {
		// Ensure there is a backslash at the end
		if (input.back() != '\\' && input.back() != '/') {
			input += "\\";
		}
	}
}
