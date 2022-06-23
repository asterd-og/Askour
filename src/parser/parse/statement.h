#pragma once

#include <stdint.h>
#include <parser/parser.h>
#include <map>
#include <vector>
#include <string>

namespace parser {
    void parse_statement(std::vector<node>* ast);
}