#include <boost/assign/list_of.hpp>
#include <map>
#include "Command.hpp"

namespace gtp {

bool is_command(const std::string &str) {
	return string_to_command(str) != CMD_UNKNOWN;
}

Command string_to_command(const std::string &str) {
	std::map<std::string, Command> map = boost::assign::map_list_of
		("protocol_version",	CMD_PROTOCOL_VERSION)
		("name", 				CMD_NAME)
		("version",				CMD_VERSION)
		("known_command",		CMD_KNOWN_COMMAND)
		("list_commands",		CMD_LIST_COMMANDS)
		("quit",				CMD_QUIT)
		("boardsize",			CMD_BOARDSIZE)
		("clear_board",			CMD_CLEAR_BOARD)
		("komi",				CMD_KOMI)
		("play",				CMD_PLAY)
		("genmove",				CMD_GENMOVE);

	try {
		return map.at(str);
	} catch (std::out_of_range e) {
		return CMD_UNKNOWN;
	}
}

std::vector<std::string> all_commands() {
	std::vector<std::string> arr = {
		"unknown",
		"protocol_version",
		"name",
		"version",
		"known_command",
		"list_commands",
		"quit",
		"boardsize",
		"clear_board",
		"komi",
		"play",
		"genmove"
	};

	return arr;
}

std::string command_to_string(const Command &c) {
	return all_commands()[c];
}

}
