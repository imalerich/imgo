#include "Move.hpp"

namespace gtp {

ArgMove::ArgMove(std::string color, std::string vertex) {
	iArgument::type = ARG_MOVE;
	this->color = Color(color);
	this->vertex = Vertex(vertex);
}

ArgMove::ArgMove(Color color, Vertex vertex) {
	iArgument::type = ARG_MOVE;
	this->color = color;
	this->vertex = vertex;
}

}
