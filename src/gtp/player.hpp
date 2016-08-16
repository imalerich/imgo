#include <string>

namespace gtp {

/**
 * Representation of the Gnu Text Protocol concept of a move.
 * Renamed to player to make it slightly less ambiguous.
 */
enum Player {
	BLACK = 0,
	WHITE = 1
};

/**
 * Convert a string as returned by the Gnu Text Protocol to 
 * the enumerated Player type. If 'str' cannot be parsed,
 * BLACK will be returned by default.
 * \param String to parse into a Player enumerated object.
 * \return Player representation of the input string.
 */
Player stringToPlayer(std::string str);

}
