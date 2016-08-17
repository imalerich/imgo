#ifndef MOVE_HPP
#define MOVE_HPP

#include "Argument.hpp"
#include "Player.hpp"
#include "Vertex.hpp"

namespace gtp {

/** Implements the Move argument type. */
class ArgMove : public iArgument {
public:
	ArgMove(std::string color, std::string vertex);
	ArgMove(Color color, Vertex vertex);

	/** Encapsulated Color data. */
	Color color;
	/** Encapsulated Vertex data. */
	Vertex vertex;
};

}

#endif
