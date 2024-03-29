#pragma once

#include <lexer/lexer.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

namespace parser {
    typedef enum {
        N_EOF=0,
        N_var_def,
        N_var_fcall_def,
        N_var_modify,
        N_func_def,
        N_func_call,
        N_extern,
        N_global,
        N_ret,
        N_if,
        N_else,
        N_expr //well... now the fun begins!
    } node_type;

    struct val {
        std::string data;

        lexer::tok_type type;
    };

    struct cond {
        val left;
        val middle;
        val right;
    };

    struct expr {
        lexer::tok_type type;
        val left;
        val right;
        val value;
        bool _continue;
    };

    struct node {
        node_type type;

        int line;
        int col;

        val left;
        val middle;
        val right;
        val value;

        std::vector<node> inner_nodes;

        bool ptr;

        std::map<std::string, std::string> arguments;
        std::vector<val> cargs;

        cond condition;

        std::vector<expr> _expr;
    };

    void error(std::string fmt, ...);
    void init(std::vector<lexer::token> toks);
    void peek(std::vector<node> ast);
    std::vector<node> parse();
    lexer::token eat(lexer::tok_type token);
}