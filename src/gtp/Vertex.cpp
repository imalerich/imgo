#include <boost/lexical_cast.hpp>
#include <sstream>
#include <ctype.h>
#include "Vertex.hpp"

namespace gtp {

Vertex::Vertex(std::string v) {
	iArgument::type = ARG_VERTEX;
	char h;
	std::istringstream iss(v);
	iss >> h >> v_pos;
	h_pos = toupper(h) - 'A';
}

std::pair<unsigned, unsigned> Vertex::coords() {
	return std::make_pair(h_pos, v_pos);
}

}
