#include <codegen/gen.h>
#include <codegen/gens/func.h>
#include <codegen/gens/var.h>

namespace gen {
    using namespace parser;

    std::vector<node> ast;
    std::map<std::string, std::string> fasm;
    std::string current_func;

    std::map<node_type, void(*)(node, int, std::vector<node>)> funcs;

    void error(std::string fmt, ...) {
        va_list args;
        std::string bf;
        va_start(args, fmt);
        vsprintf(bf.data(), fmt.c_str(), args);
        va_end(args);
        printf("[Gen]: Error: %s\n", bf.data());
        exit(1);
    }

    void warn(std::string fmt, ...) {
        va_list args;
        std::string bf;
        va_start(args, fmt);
        vsprintf(bf.data(), fmt.c_str(), args);
        va_end(args);
        printf("[Gen]: Warning: %s\n", bf.data());
    }

    void set_current_func(std::string name) {
        current_func=name;
    }

	std::string get_current_func() {
        return current_func;
    }

    void init(std::vector<node> gast) {
        ast = gast;

        funcs[N_func_def] = &func::def;
        funcs[N_func_call] = &func::call;
        funcs[N_var_def]  = &var::gen;
    }

    void emit(std::string fmt) {
        fasm[current_func].append(fmt.c_str());
        fasm[current_func].append("\n");
    }

    int peek(int index, std::vector<node> ast) {
        if ( ast[index].type != node_type::N_EOF ) funcs[ast[index].type](ast[index], index, ast);
    }

    std::string run() {
        set_current_func(OUTSIDE);
        emit(".intel_syntax noprefix");
        emit(".global main\n");

        set_current_func("_$.EXIT");
        emit("\tmov rax, 60");
        emit("\tmov rdi, 0");
        emit("\tsyscall");

        if ( ast[0].type != node_type::N_EOF ) {
            for ( int i = 0; i < ast.size(); i++ ) {
                peek(i, ast);
            }
        }

        std::string out;

        out.append(fasm[OUTSIDE]);

        for ( auto piece : fasm ){
            if ( piece.first != OUTSIDE ) {
                out.append(piece.first);
                out.append(":\n");
                out.append(piece.second);
                if (!func::is_str(piece.first) && piece.first!="_$.EXIT") {
                    out.append("\tnop\n");
                    out.append("\tpop rbp\n");
                    if (piece.first=="main") {
                        out.append("\tjmp _$.EXIT\n");
                    } else {
                        out.append("\tret\n");
                    }
                }
            }
        }

        return out;
    }
}