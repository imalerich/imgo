#include <string>
#include <utility>

namespace gtp {

/**
 * Vertex representation for the Gnu Text Protocol.
 * This object make be initialized from a string in the format
 * A10, where A is the horizontal position of the vertex (Left->Right)
 * and 10 is the Vertical Position of the vertex (Bottom->Top).
 */
class Vertex {
public:
	/**
	 * Constructs a vertex object from the string representation returned by 
	 * the Gnu Text Protocol.
	 */
	Vertex(std::string v);

	/**
	 * Returns the coordinate representation of this vertex as a
	 * pair of unsigned integers.
	 */
	std::pair<unsigned, unsigned> coords();

private:
	/// Horizontal position of this vertex where 0 is the left edge of the board.
	unsigned h_pos;
	/// Vertical position of this vertex, where 0 is the bottom edge of the board.
	unsigned v_pos;
}

}
