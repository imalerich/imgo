#include <string>

namespace gtp {

/**
 * Representation of the Gnu Text Protocol concept of a move.
 * Renamed to player to make it slightly less ambiguous.
 */
enum Player {
	BLACK,
	WHITE
};

/**
 * Convert a string as returned by the Gnu Text Protocol to 
 * the enumerated Player type. If 'str' cannot be parsed,
 * BLACK will be returned by default.
 * \param String to parse into a Player enumerated object.
 * \return Player representation of the input string.
 */
Player stringToPlayer(const std::string str);

/**
 * \p Object of the Player enumerated type.
 * \return A string representation of that enum supported by the gtp.
 */
std::string playerToString(const Player p);

}
