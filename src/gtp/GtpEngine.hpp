#include <iostream>
#include <string>

namespace gtp {

/**
 * An engine for the Go Text Protocol.
 * An instance of this class may be used to communicate between
 * with the go server that maintains the current game. 
 */
class GtpEngine {
public:

private:
	/**
	 * Performs preprocessing on a line of text.
	 * The following operation are performed in accordance with the GTP specification.
	 * 1. Remove all occurences of CR and other control characters except for HT and LF.
	 * 2. For each line with a hash sign (#), remove all text following and including this characeter.
	 * 3. Convert all occurences of HT to SPACE.
	 * 4. Discard any empty or white-space only lines.
	 * \param line to the line to be processed.
	 * \return Processed version of the input string.
	 */
	std::string preproc_line(std::string line);

	/**
	 * \param The line in question.
	 * \return Should this line be ignored?
	 */
	bool ignore_line(const std::string line);
}

}
