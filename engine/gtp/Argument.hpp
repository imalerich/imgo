#ifndef ARGUMENT_HPP
#define ARGUMENT_HPP

#include <boost/lexical_cast.hpp>
#include <string>
#include <vector>
#include <memory>

namespace gtp {

/**
 * Specifies the type of argument that 
 * was received from the controller.
 * This may be used to retrieve the corresponding
 * data from the argument list.
 */
enum ArgType {
	ARG_INTEGER,
	ARG_FLOAT,
	ARG_STRING,
	ARG_VERTEX,
	ARG_COLOR,
	ARG_MOVE,
	ARG_BOOLEAN
};

/**
 * Base argmument type, used to generate arrays of arguments.
 */
class iArgument {
public:
	virtual ~iArgument() { }
	ArgType type; /**< What type of data is stored? */
};

/** An argument list is an array of pointers to the argument interface. */
typedef std::vector<std::unique_ptr<iArgument>> ARG_LIST;

/* ----------------------------------------------------------
 Argument Type Implementations.
 ----------------------------------------------------------*/

/** Implements the Integer argument type. */
class ArgInteger : public iArgument {
public:
	ArgInteger(std::string str);

	/** Encapsulated integer data. */
	int data;
};

/** Implements the Integer argument type. */
class ArgFloat : public iArgument {
public:
	ArgFloat(std::string str);

	/** Encapsulated float data. */
	float data;
};

/** Implements the String argument type. */
class ArgString : public iArgument {
public:
	ArgString(std::string str);

	/** Encapsulated string data. */
	std::string data;
};

/** Implements the Boolean argument type. */
class ArgBoolean : public iArgument {
public:
	ArgBoolean(std::string str);

	/** Encapsulated bool data. */
	bool data;
};

}

#endif
