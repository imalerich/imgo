#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "Command.hpp"
#include "Argument.hpp"

namespace gtp {

/**
 * Defines a function that may be registered to the GTP Engine.
 * When an engine recives the corresponding Command, the registered
 * function will be called. This function should return the string
 * representing the result of the function, it will then be output by the
 * Engine (including any additional data needed by the controller).
 */
typedef std::string (*ProcCmd)(const ARG_LIST&);

/**
 * An engine for the Go Text Protocol.
 * An instance of this class may be used to communicate between
 * with the go server that maintains the current game. 
 */
class Engine {
public:
	/**
	 * Constructs an Engine by registering the default implementation
	 * for each supported command.
	 */
	Engine();

	/**
	 * Main game loop for the engine.
	 * This will read and execute commands from the given input stream
	 * until either an EOF is sent or the 'quit' command is given.
	 * \param is Input stream to read commands from.
	 */
	void play(std::istream &is);

	/**
	 * Read and execute a single command from the input stream.
	 * \param is Input stream to read a command from.
	 */
	void proc_command(std::istream &is);

	/**
	 * Registers the input proc to the given command.
	 * Whenever that command is received from the controller,
	 * the proc function will be executed with the arguments
	 * as supplied by the controller.
	 * If a proccess is already registered to the input Command,
	 * that process will be overridden.
	 * \param cmd Command to register the input proc to.
	 * \param proc The process to execute on receiving the given command.
	 */
	void register_proc(Command cmd, const ProcCmd &proc);

private:
	std::map<Command,ProcCmd> commands;

	/**
	 * Parse all arguments from the input line, given the type of argument that will be parsed.
	 * \param cmd Type of command to parse arguments for.
	 * \param iss Input string stream of the line being parsed.
	 * \return Array of pointers to argument objects.
	 */
	ARG_LIST args_for_cmd(const Command &cmd, std::istringstream &iss);

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
	std::string preproc_line(const std::string &line);

	/**
	 * \param The line in question.
	 * \return Should this line be ignored?
	 */
	bool ignore_line(const std::string &line);
};

}

#endif
