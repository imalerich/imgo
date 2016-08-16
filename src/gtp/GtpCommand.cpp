#include <boost/assign/list_of.hpp>
#include <map>
#include "GtpCommand.hpp"

namespace gtp {

Command stringToCommand(const std::string str) {
	std::map<std::string, Command> map = boost::assign::map_list_of
		("unknown", UNKNOWN)
		("protocol_version", PROTOCOL_VERSION)
		("name", NAME)
		("version", VERSION)
		("known_command", KNOWN_COMMAND)
		("list_commands", LIST_COMMANDS)
		("quit", QUIT)
		("boardsize", BOARDSIZE)
		("clear_board", CLEAR_BOARD)
		("komi", KOMI)
		("play", PLAY)
		("genmove", GENMOVE);

	return map[str];
}

std::string commandToString(const Command c) {
	std::string arr[] = {
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

	return arr[c];
}

}
