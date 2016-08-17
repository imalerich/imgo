#include <ctype.h>
#include <memory>
#include <stdlib.h>
#include <time.h>

#include "Defaults.hpp"
#include "Engine.hpp"
#include "Move.hpp"

namespace gtp {

Engine::Engine() {
	srand(time(NULL));
	register_proc(CMD_PROTOCOL_VERSION, &protocol_version);
	register_proc(CMD_NAME, &name);
	register_proc(CMD_VERSION, &version);
	register_proc(CMD_KNOWN_COMMAND, &known_command);
	register_proc(CMD_LIST_COMMANDS, &list_commands);
	register_proc(CMD_QUIT, &quit);
	register_proc(CMD_BOARDSIZE, &boardsize);
	register_proc(CMD_CLEAR_BOARD, &clear_board);
	register_proc(CMD_KOMI, &do_nothing);
	register_proc(CMD_PLAY, &do_nothing);
	register_proc(CMD_GENMOVE, &genmove);
}

void Engine::play(std::istream &is) {
	while (!is.eof()) {
		engine.proc_command(is);
	}
}

void Engine::proc_command(std::istream &is) {
	std::string line;
	getline(is, line);

	line = preproc_line(line);
	if (ignore_line(line)) {
		return;
	}

	std::string id = " ", cmd_str;
	std::istringstream iss(line);
	iss >> cmd_str;
	if (!is_command(cmd_str)) {
		bool is_number = true;
		for (auto c : cmd_str) { is_number = is_number && isdigit(c); }
		id = is_number ? cmd_str + " " : id;
		iss >> cmd_str;
	}

	Command cmd = string_to_command(cmd_str);
	ARG_LIST args = args_for_cmd(cmd, iss);

	try {
		ProcCmd proc = commands.at(cmd);
		std::string out = proc(args);
		std::cout << "=" << id << proc(args) << "\n\n";
	} catch (std::out_of_range e) { 
		std::cout << "?" << id << "unknown command\n\n";
	}
}

void Engine::register_proc(Command cmd, const ProcCmd &proc) {
	commands[cmd] = proc;
}

ARG_LIST Engine::args_for_cmd(const Command &cmd, std::istringstream &iss) {
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
			return ret;
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
