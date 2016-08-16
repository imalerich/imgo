#include <boost/algorithm/string.hpp>
#include "Player.hpp"

namespace gtp {

Player string_to_player(const std::string str) {
	if (str == "w" || str == "W" || "WHITE" == boost::to_upper_copy<std::string>(str)) {
		return PLAYER_WHITE;
	}

	return PLAYER_BLACK;
}

std::string player_to_string(const Player p) {
	std::string arr[] = { "B", "W" };
	return arr[p];
}

Color::Color(Player p) {
	iArgument::type = ARG_MOVE;
	this->p = p;
}

Color::Color(const std::string str) {
	iArgument::type = ARG_MOVE;
	p = string_to_player(str);
}

}
