#include <boost/algorithm/string.hpp>
#include "move.hpp"

namespace gtp {

Player stringToPlayer(std::string str) {
	if (str == "w" || str == "W" || "WHITE" == boost::to_upper_copy<std::string>(str)) {
		return WHITE;
	}

	return BLACK;
}

}
