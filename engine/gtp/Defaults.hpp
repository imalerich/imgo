#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP

#include <string>
#include <vector>
#include <stdlib.h>
#include "Argument.hpp"
#include "Vertex.hpp"
#include "Command.hpp"

namespace gtp {

/// Store a static copy of the board size, this will be used to generate random moves.
static int board_size = 0;
/// A simple representation of the game board, 0 means no stone is present, 
/// 1 means a stone is present. This will be used so we don't generate any repeat moves.
static std::vector<int> board;

std::string protocol_version(const ARG_LIST &args) {
	return "2";
}

std::string name(const ARG_LIST &args) {
	return "ImGo::GTP";
}

std::string version(const ARG_LIST &args) {
	return "0.1";
}

std::string known_command(const ARG_LIST &args) {
	ArgString * s = (ArgString *)args.front().get();
	std::string cmd = s->data;
	return is_command(cmd) ? "true" : "false";
}

std::string list_commands(const ARG_LIST &args) {
	std::string ret;
	for (auto str : all_commands()) {
		ret += (str + "\n");
	}

	return ret;
}

std::string quit(const ARG_LIST &args) {
	exit(0);
}

std::string boardsize(const ARG_LIST &args) {
	ArgInteger * i = (ArgInteger *)args.front().get();
	board_size = i->data;

	board.clear();
	for (auto i=0; i<(board_size * board_size); i++) {
		board.push_back(0);
	}

	return "";
}

std::string genmove(const ARG_LIST &args) {
	for (int count = 0; true; count++) {
		int x = std::rand() % board_size;
		int y = std::rand() % board_size;

		if (board[y * 19 + x] == 0) {
			board[y * 19 + x] = 1;
			return Vertex(x, y).to_string();
		} else if (count > 20) { 
			return "pass"; 
		}
	}
}

std::string play(const ARG_LIST &args) {
	Vertex * v = (Vertex *)args[1].get();
	std::pair<int, int> coords = v->coords();
	board[coords.second * 19 + coords.first] = 1;
	return "";
}

std::string clear_board(const ARG_LIST &args) {
	for (auto &i : board) { i = 0; }
	return "";
}

std::string do_nothing(const ARG_LIST &args) { 
	return "";
}

}

#endif
