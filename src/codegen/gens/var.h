#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <map>
#include <parser/parser.h>

namespace var {
    typedef enum {
        dword=0,
        qword,
        byte
    } word;

    int         get_type(std::string name);
    bool        type_exists(std::string type);

    bool        exists(std::string name);
    bool        is_ptr(std::string name);

    word        get_word(std::string name);
    std::string word2str(word w);

    void        set_word(std::string name, word w);
    void        set_ptr(std::string name, bool p);
    void        set_type(std::string name, std::string type);

    void        _new(std::string type, std::string name, std::string value, bool string, bool ptr);
    int         get(std::string name);
    void        set(std::string name, int index);
    
    void        set_stack(int stack);
    int         get_stack();
    void        update_stack(std::string type);

    int         get_total_stack();
    void        parse_total_stack(parser::node ast);
    
    void        gen(parser::node node, int index, std::vector<parser::node> ast);
}