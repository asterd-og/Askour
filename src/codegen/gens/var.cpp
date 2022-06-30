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

    bool is_normal_math(std::vector<parser::expr> _expr) {
        for (parser::expr _e : _expr) {
            switch (_e.type) {
                case lexer::tok_type::T_ID:
                    return false;
                    break;
            }
        }
        return true;
    }

    bool is_math(std::vector<parser::expr> _expr) {
        if (_expr[0].type==lexer::tok_type::T_ID || _expr[0].type==lexer::tok_type::T_NUM || _expr[0].type==lexer::tok_type::T_STR) {
            //basically, if the calc type is ID/NUM
            //which in this case, would be ADD or SUB
            //its just an ID/NUM reference
            return false;
        }
        return true;
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
        int value=0;
        std::string _value;
        if (is_math(node._expr)) {
            //for now, only handle this
            if (is_normal_math(node._expr)) {
                for (parser::expr _e : node._expr) {
                    switch (_e.type) {
                        //I am not proud of the code ahead.
                        case lexer::tok_type::T_ADD:
                            if (_e._continue==false) {
                                value=std::stoi(_e.left.data)+std::stoi(_e.right.data);
                            } else {
                                value+=std::stoi(_e.left.data);
                            }
                            break;
                        case lexer::tok_type::T_SUB:
                            if (_e._continue==false) {
                                value=std::stoi(_e.left.data)-std::stoi(_e.right.data);
                            } else {
                                value-=std::stoi(_e.left.data);
                            }
                            break;
                        case lexer::tok_type::T_DIV:
                            if (_e._continue==false) {
                                value=std::stoi(_e.left.data)/std::stoi(_e.right.data);
                            } else {
                                value/=std::stoi(_e.left.data);
                            }
                            break;
                        case lexer::tok_type::T_MUL:
                            if (_e._continue==false) {
                                value=std::stoi(_e.left.data)*std::stoi(_e.right.data);
                            } else {
                                value*=std::stoi(_e.left.data);
                            }
                            break;
                    }
                }
                _value=std::to_string(value);
            } else {
                gen::error("Not implemented.");
            }
        } else {
            _value=node._expr[0].left.data;
        }
        _new(type, name, _value, (node._expr[0].type==lexer::tok_type::T_STR), node.ptr);
    }
}