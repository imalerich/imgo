#include <iostream>
#include <gtp/gtp.hpp>
#include <stdlib.h>
#include <string>

int main(int argc, char ** argv) {
	gtp::Engine engine;
	while (!std::cin.eof()) {
		engine.proc_command(std::cin);
	}
	
	return 0;
}
