#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <map>
#include <parser/parser.h>

namespace var {
    int         get_type(std::string name);
    bool        type_exists(std::string type);

    bool        exists(std::string name);
    bool        is_ptr(std::string name);

    void        _new(std::string type, std::string name, std::string value, bool string, bool ptr);
    int         get(std::string name);
    
    void        set_stack(int stack);
    int         get_stack();
    void        update_stack(std::string type);
    
    void        gen(parser::node node, int index, std::vector<parser::node> ast);
}