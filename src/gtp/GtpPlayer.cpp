#include <boost/algorithm/string.hpp>
#include "GtpPlayer.hpp"

namespace gtp {

Player stringToPlayer(const std::string str) {
	if (str == "w" || str == "W" || "WHITE" == boost::to_upper_copy<std::string>(str)) {
		return WHITE;
	}

	return BLACK;
}

std::string playerToString(const Player p) {
	std::string arr[] = { "B", "W" };
	return arr[p];
}

}
