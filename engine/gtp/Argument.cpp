#include "Argument.hpp"

namespace gtp {

ArgInteger::ArgInteger(std::string str) {
	iArgument::type = ARG_INTEGER;
	data = boost::lexical_cast<int>(str);
}

ArgFloat::ArgFloat(std::string str) {
	iArgument::type = ARG_FLOAT;
	data = boost::lexical_cast<float>(str);
}

ArgString::ArgString(std::string str) {
	iArgument::type = ARG_STRING;
	data = str;
}

ArgBoolean::ArgBoolean(std::string str) {
	iArgument::type = ARG_BOOLEAN;
	data = boost::lexical_cast<bool>(str);
}

}
