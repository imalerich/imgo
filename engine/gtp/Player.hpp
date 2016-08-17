#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "Argument.hpp"

namespace gtp {

/**
 * Representation of the Gnu Text Protocol concept of a move.
 * Renamed to player to make it slightly less ambiguous.
 */
enum Player {
	PLAYER_BLACK,
	PLAYER_WHITE
};

/**
 * Convert a string as returned by the Gnu Text Protocol to 
 * the enumerated Player type. If 'str' cannot be parsed,
 * BLACK will be returned by default.
 * \param String to parse into a Player enumerated object.
 * \return Player representation of the input string.
 */
Player string_to_player(const std::string str);

/**
 * \p Object of the Player enumerated type.
 * \return A string representation of that enum supported by the gtp.
 */
std::string player_to_string(const Player p);

/**
 * Implements the Argument interface for the Player enumerated type.
 */
class Color : public iArgument {
public:
	/** Needed so we can store it as a non-reference in the move container. */
	Color() { }
	/** Initialize directly from the Player enumrated type. */
	Color(Player p);
	/** Initialize from a string that is converted to the Player enumerated type. */
	Color(const std::string str);

	/** Encapsulated player data. */
	Player p;
};

}

#endif
