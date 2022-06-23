#pragma once

#include <stdint.h>
#include <parser/parser.h>
#include <map>
#include <vector>
#include <string>

namespace parser {
    void parse_funcdef(std::string type, std::string name, std::vector<parser::node>* ast);
    void parse_funccall(std::string name, std::vector<parser::node>* ast);
}