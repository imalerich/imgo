#ifndef MOVE_HPP
#define MOVE_HPP

#include "Argument.hpp"
#include "Player.hpp"
#include "Vertex.hpp"

namespace gtp {

/** Implements the Move argument type. */
class ArgMove : public iArgument {
public:
	ArgMove(std::string color, std::string vertex) {
		iArgument::type = ARG_MOVE;
		this->color = Color(color);
		this->vertex = Vertex(vertex);
	}

	ArgMove(Color color, Vertex vertex) {
		iArgument::type = ARG_MOVE;
		this->color = color;
		this->vertex = vertex;
	}

	/** Encapsulated Color data. */
	Color color;
	/** Encapsulated Vertex data. */
	Vertex vertex;
};

}

#endif
