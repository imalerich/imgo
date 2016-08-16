#include <ctype.h>
#include "GtpEngine.hpp"

namespace gtp {

std::string Engine::preproc_line(std::string line) {
	// We will build up this string from processing 'line', then return it.
	std::string ret;

	for (auto c : line) {
		switch (c) {
		// Ignored control characters.
		case '\v':
		case '\b':
		case '\r':
		case '\f':
		case '\a':
		case '\0':
			continue;
		case '\t':
			ret.push_back(' ');
		case '#':
			break;
		default:
			ret.push_back(c);
		}
	}

	return ret;
}

bool Engine::ignore_line(const std::string line) {
	bool only_space = true;
	for (auto c : line) {
		only_space = only_space && isspace(c);
	}

	return only_space ? true : line.empty();
}

}
