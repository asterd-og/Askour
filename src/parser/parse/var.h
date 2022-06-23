#pragma once

#include <stdint.h>
#include <parser/parser.h>
#include <map>
#include <vector>
#include <string>

namespace parser {
    void parse_vardef(std::string type, std::string name, std::vector<node>* ast);
}