#include <codegen/gens/func.h>
#include <codegen/gens/string.h>
#include <codegen/utils.h>
#include <codegen/gens/var.h>
#include <codegen/gen.h>
#include <stdbool.h>

namespace func {
    std::map<std::string, bool> fs;

    std::string regs[]={"rdi",
                        "rsi",
                        "rdx",
                        "rcx",
                        "r8",
                        "r9",
                        "r10",
                        "r11",
                        "r12",
                        "r13",
                        "r14",
                        "r15",};

    void _new(std::string name, bool str) {
        fs[name] = str;
        gen::fasm[name] = "";
    }

    bool exists(std::string name) {
        return ( fs.find(name) != fs.end() );
    }

    bool is_str(std::string name) {
        return fs[name];
    }

    void def(parser::node node, int index, std::vector<parser::node> ast) {
        std::string name=node.middle.data;

        if (exists(name)) {
            gen::error("Function %s already exists", name.c_str());
        }

        _new(name, false);

        gen::set_current_func(name);

        var::parse_total_stack(node);

        gen::emit("\tpush rbp");
        gen::emit("\tmov rbp, rsp");

        if ( node.arguments.size() > 0 ) {
            int index_reg=0;

            int total_stack=var::get_total_stack();

            int stack=total_stack*2;

            for (auto argument : node.arguments) {
                stack+=var::get_type(argument.second);

                // \/ isnt the best code, but ill let it there for now
                var::word w=(var::get_type(argument.second)==1 ? var::word::byte : (var::get_type(argument.second)==8 ? var::word::qword : var::word::dword));
                gen::emit(utils::fmt("\tmov %s ptr [rbp-%d], %s", var::word2str(w).c_str(), stack, regs[index_reg].c_str()));
                
                //setup the variable
                var::set(argument.first, stack);
                var::set_ptr(argument.first, false);
                var::set_word(argument.first, w);
                var::set_type(argument.first, argument.second);

                index_reg++;
            }

        }
        var::set_stack(0);

        if ( node.inner_nodes.size() > 0 ) {
            for ( int i = 0; i < node.inner_nodes.size(); i++ ) {
                gen::peek(i, node.inner_nodes);
            }
        }
    }

    void call(parser::node node, int index, std::vector<parser::node> ast) {
        std::string name=node.left.data;

        if (!exists(name)) {
            gen::warn("Implicit declaration of function '%s'.", name.c_str());
        }

        if (node.cargs.size() > 0) {
            int reg_index=0;

            for (parser::val arg : node.cargs) {
                if (arg.type == lexer::tok_type::T_ID) {
                    if (var::exists(arg.data)) {
                        gen::emit(utils::fmt("\tmov rax, %s ptr[rbp-%d]",  (var::word2str(var::get_word(arg.data))).c_str(), var::get(arg.data)));
                        gen::emit(utils::fmt("\tmov %s, rax", regs[reg_index].c_str()));
                    } else {
                        gen::error("Undefined name '%s' at %d:%d.\n", node.line, node.col);
                    }
                } else if (arg.type == lexer::tok_type::T_STR) {
                    std::string fname=string::_new(arg.data);
                    gen::emit(utils::fmt("\tmov rax, offset flat:%s", fname.c_str()));
                    gen::emit(utils::fmt("\tmov %s, rax", regs[reg_index].c_str()));
                } else {
                    gen::emit(utils::fmt("\tmov %s, %s", regs[reg_index].c_str(), arg.data.c_str()));
                }
            }
        }

        gen::emit(utils::fmt("\tcall %s", name.c_str()));
    }
}