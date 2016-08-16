#include <ctype.h>
#include <sstream>
#include <memory>
#include "Engine.hpp"
#include "Move.hpp"

namespace gtp {

ARG_LIST Engine::args_for_cmd(const Command &cmd, const std::string &line) {
	std::istringstream iss(line);
	ARG_LIST args;

	switch (cmd) {
		case CMD_KNOWN_COMMAND: {
			std::string command_name;
			iss >> command_name;
			args.push_back(std::make_unique<ArgString>(command_name));
			}
			break;

		case CMD_BOARDSIZE: {
			std::string size;
			iss >> size;
			args.push_back(std::make_unique<ArgInteger>(size));
			}
			break;

		case CMD_KOMI: {
		   std::string new_komi;
			iss >> new_komi;
			args.push_back(std::make_unique<ArgFloat>(new_komi));
			}
			break;

		case CMD_PLAY: {
			std::string color;
			std::string vertex;
			iss >> color >> vertex;
			args.push_back(std::make_unique<ArgMove>(color, vertex));
			}
			break;

		case CMD_GENMOVE: {
			std::string color;
			iss >> color;
			args.push_back(std::make_unique<Color>(color));
			break;
			}

		default:
			break;
	}

	return args;
}

std::string Engine::preproc_line(const std::string &line) {
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

bool Engine::ignore_line(const std::string &line) {
	bool only_space = true;
	for (auto c : line) {
		only_space = only_space && isspace(c);
	}

	return only_space ? true : line.empty();
}

}
