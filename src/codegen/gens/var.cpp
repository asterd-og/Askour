#include <codegen/gens/var.h>
#include <codegen/gens/func.h>
#include <codegen/gens/string.h>
#include <codegen/gen.h>
#include <codegen/utils.h>
#include <lexer/lexer.h>
#include <stdbool.h>

namespace var {
    std::map<std::string, std::map<std::string, int>>         vars;
    //first comes the function name, then the var name, and its index.
    std::map<std::string, std::map<std::string, word>>        vars_words;
    std::map<std::string, std::map<std::string, bool>>        vars_ptrs;
    std::map<std::string, std::map<std::string, std::string>> vars_type;
    int last_stack=0;

    std::map<std::string, int>                                total_stack;

    std::map<std::string, int> types = {
        {"int", 4},
        {"float", 8},
        {"char", 1},
        {"bool", 1},
        {"v0", 0},
        {"u8t", 1},
        {"string", 8}
    };

    std::map<word, std::string> words2str = {
        { word::dword, "dword" },
        { word::qword, "qword" },
        { word::byte, "byte" },
    };

    int get_type(std::string type) {
        return types[type];
    }

    bool type_exists(std::string type) {
        return ( types.find(type) != types.end() );
    }

    bool exists(std::string name) {
        return ( vars[gen::get_current_func()].find(name) != vars[gen::get_current_func()].end() );
    }

    bool is_ptr(std::string name) {
        return vars_ptrs[gen::get_current_func()][name];
    }

    word get_word(std::string name) {
        return vars_words[gen::get_current_func()][name];
    }

    std::string word2str(word w) {
        return words2str[w];
    }

    void set_word(std::string name, word w) {
        vars_words[gen::get_current_func()][name]=w;
    }

    void set_ptr(std::string name, bool p) {
        vars_ptrs[gen::get_current_func()][name]=p;
    }

    void set_type(std::string name, std::string type) {
        vars_type[gen::get_current_func()][name]=type;
    }

    void set(std::string name, int index) {
        vars[gen::get_current_func()][name]=index;
    }

    void _new(std::string type, std::string name, std::string value, bool str, bool ptr) {
        if (type_exists) {
            update_stack(type);
            set_type(name, type);
            set_ptr(name, ptr);
            set(name, get_stack());

            if (str) {
                set_word(name, word::qword);
                std::string fname=string::_new(value);
                gen::emit(utils::fmt("\tmov qword ptr[rbp-%d], offset flat:%s", get_stack(), fname.c_str()));
            } else {
                if (ptr) {
                    set_word(name, word::qword); //if its a pointer
                    //ignore all the other if statements
                } else {
                    if (types[type]==1) {
                        set_word(name, word::byte);
                    } else {
                        set_word(name, word::dword);
                    }
                }
                
                gen::emit(utils::fmt("\tmov %s ptr[rbp-%d], %s", word2str(get_word(name)).c_str(), get_stack(), value.c_str()));
            }
        } else {
            gen::error("Unknown type %s", type.c_str());
        }
    }

    int get(std::string name) {
        return vars[gen::get_current_func()][name];
    }

    void set_stack(int stack) {
        last_stack=stack;
    }

    int get_stack() {
        return last_stack;
    }

    void update_stack(std::string type) {
        last_stack+=get_type(type);
    }

    int get_total_stack() {
        return total_stack[gen::get_current_func()];
    }

    void parse_total_stack(parser::node fbody) {
        for (parser::node node : fbody.inner_nodes) {
            if (node.type==parser::node_type::N_var_def) {
                total_stack[fbody.middle.data]+=get_type(node.left.data);
            }
        }
    }

    void gen(parser::node node, int index, std::vector<parser::node> ast) {
        std::string name=node.middle.data;
        std::string type=node.left.data;
        std::string value=node.value.data;
        _new(type, name, value, (node.value.type==lexer::tok_type::T_STR), node.ptr);
    }
}