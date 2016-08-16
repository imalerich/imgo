#include <string>

namespace gtp {

/**
 * List of all supported commands by our Gtp Engine.
 */
enum Command {
	UNKNOWN,
	PROTOCOL_VERSION,
	NAME,
	VERSION,
	KNOWN_COMMAND,
	LIST_COMMANDS,
	QUIT,
	BOARDSIZE,
	CLEAR_BOARD,
	KOMI,
	PLAY,
	GENMOVE
};

/**
 * \param str String to parse into a valid Gtp Command.
 * \return An object of the Command enumerated type.
 */
Command stringToCommand(const std::string str);

/**
 * \praam c Command object to converted into a valid string representation.
 * \return A string object for that command that may be sent through gtp.
 */
std::string commandToString(const Command c);

}
