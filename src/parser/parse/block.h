#pragma once

#include <stdint.h>
#include <parser/parser.h>
#include <map>
#include <vector>
#include <string>

namespace parser {
    std::vector<node> parse_block(std::vector<node>* ast);
}