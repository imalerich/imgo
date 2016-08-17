#include <boost/lexical_cast.hpp>
#include <string>
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

Vertex::Vertex(int x, int y) {
	iArgument::type = ARG_VERTEX;
	h_pos = x;
	v_pos = y;
}

std::pair<int, int> Vertex::coords() {
	return std::make_pair(h_pos, v_pos);
}

std::string Vertex::to_string() {
	char arr[] = { (char)('A' + h_pos), '\0' };
	return std::string(arr) + std::to_string(v_pos + 1);
}

}
