#include <codegen/gens/var.h>
#include <codegen/gens/func.h>
#include <codegen/gen.h>
#include <codegen/utils.h>
#include <lexer/lexer.h>


namespace var {
    std::map<std::string, int>           vars;
    std::map<std::string, std::string>   vars_type;
    int last_stack=0;
    int last_tstr=0;

    std::map<std::string, int> types = {
        {"int", 4},
        {"float", 8},
        {"char", 1},
        {"bool", 1},
        {"v0", 0},
        {"string", 8}
    };

    int get_type(std::string type) {
        return types[type];
    }

    bool type_exists(std::string type) {
        return ( types.find(type) != types.end() );
    }

    bool exists(std::string name) {
        return ( vars.find(name) != vars.end() );
    }

    void _new(std::string type, std::string name, std::string value, bool string, bool ptr) {
        if (type_exists) {
            update_stack(type);
            vars_type[name]=type;
            vars[name]=get_stack();

            if (string) {
                std::string old_fname=gen::get_current_func();
                std::string fname=utils::fmt("_$.TSTR%d", last_tstr);
                func::_new(fname.c_str(), true);
                gen::set_current_func(fname.c_str());
            
                gen::emit(utils::fmt("\t.string \"%s\"", value.c_str()));

                gen::set_current_func(old_fname);
                gen::emit(utils::fmt("\tmov qword ptr[rbp-%d], offset flat: %s", get_stack(), fname.c_str()));
            } else {
                gen::emit(utils::fmt("\tmov %s ptr[rbp-%d], %s", (ptr?"qword":"dword"), get_stack(), value.c_str()));
            }
        } else {
            gen::error("Unknown type %s", type.c_str());
        }

    }

    int get(std::string name) {
        return vars[name];
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

    void gen(parser::node node, int index, std::vector<parser::node> ast) {
        std::string name=node.middle.data;
        std::string type=node.left.data;
        std::string value=node.value.data;
        _new(type, name, value, (node.value.type==lexer::tok_type::T_STR), node.ptr);
    }
}