#include <iostream>
#include <gtp/gtp.hpp>
#include <stdlib.h>
#include <string>

std::string name(const gtp::ARG_LIST &args) {
	return "ImGo";
}

std::string protocol_version(const gtp::ARG_LIST &args) {
	return "2";
}

std::string version(const gtp::ARG_LIST &args) {
	return "0.1";
}

std::string quit(const gtp::ARG_LIST &args) {
	exit(0);
}

std::string board_size(const gtp::ARG_LIST &args) {
	gtp::ArgInteger * i = (gtp::ArgInteger *)args.front().get();
	return std::to_string(i->data);
}

int main(int argc, char ** argv) {
	gtp::Engine engine;
	engine.register_proc(gtp::CMD_NAME, &name); 
	engine.register_proc(gtp::CMD_PROTOCOL_VERSION, &protocol_version); 
	engine.register_proc(gtp::CMD_VERSION, &version); 
	engine.register_proc(gtp::CMD_QUIT, &quit); 
	engine.register_proc(gtp::CMD_BOARDSIZE, &board_size); 

	while (!std::cin.eof()) {
		engine.proc_command(std::cin);
	}
	
	return 0;
}
