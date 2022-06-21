#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <map>
#include <parser/parser.h>
#include <stdarg.h>
#include <string>
#include <memory>
#include <stdexcept>

#define OUTSIDE "$_.OUT" //. cant get lexed in a name
//so it doesnt occurs the risk of being overwritten

namespace gen {
	extern std::map<std::string, std::string> fasm;
	void error(std::string fmt, ...);
	void init(std::vector<parser::node> ast);
	std::string run();
	int peek(int index, std::vector<parser::node> ast);
	void set_current_func(std::string name);
	std::string get_current_func();
	void emit(std::string fmt);
}