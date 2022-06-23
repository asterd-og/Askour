#pragma once

#include <stdint.h>
#include <parser/parser.h>
#include <map>
#include <vector>
#include <string>

namespace parser {
    std::map<std::string, std::string> parse_arguments(std::vector<parser::node>* ast);
    std::vector<parser::val> parse_call_args(std::vector<parser::node>* ast);
}