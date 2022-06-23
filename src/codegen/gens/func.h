#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <map>
#include <string>
#include <parser/parser.h>

namespace func {
    void _new(std::string name, bool str=false);
    bool exists(std::string name);
    bool is_str(std::string name);
    void def(parser::node node, int index, std::vector<parser::node> ast);
    void call(parser::node node, int index, std::vector<parser::node> ast);
}