#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

namespace gtp {

/**
 * List of all supported commands by our Gtp Engine.
 */
enum Command {
	CMD_UNKNOWN,
	CMD_PROTOCOL_VERSION,
	CMD_NAME,
	CMD_VERSION,
	CMD_KNOWN_COMMAND,
	CMD_LIST_COMMANDS,
	CMD_QUIT,
	CMD_BOARDSIZE,
	CMD_CLEAR_BOARD,
	CMD_KOMI,
	CMD_PLAY,
	CMD_GENMOVE
};

/**
 * Check whether or not the input string is a command or not.
 * If it is not, then it is probably the provided identifier from the 
 * controller, read that as a number, and then parse the next word as
 * a command.
 * \param The first read from the controller command.
 * \return Whether or not the input 'str' is a supported command.
 */
bool is_command(const std::string &str);

/**
 * \param str String to parse into a valid Gtp Command.
 * \return An object of the Command enumerated type.
 */
Command string_to_command(const std::string &str);

/**
 * \praam c Command object to converted into a valid string representation.
 * \return A string object for that command that may be sent through gtp.
 */
std::string command_to_string(const Command &c);

}

#endif
