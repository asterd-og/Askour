#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string>
#include <vector>

namespace lexer {
    //tokens types
    typedef enum {
        T_EOF=0,
        T_NL,
        T_NUM,
        T_STR,
        T_CHAR,
        T_ID,
        T_LPAR,
        T_RPAR,
        T_LBRACE,
        T_RBRACE,
        T_LSBRACE,
        T_RSBRACE,
        T_STAR,
        T_SEMI,
        T_COMMA,
        T_EQEQ,
        T_LTEQ,
        T_LT,
        T_GTEQ,
        T_GT,
        T_NEQ,
        T_EQ,
        T_COLON,
        T_ADDEQ,
        T_SUBEQ,
        T_DIVEQ,
        T_MULEQ,
        T_ADD,
        T_SUB,
        T_DIV,
        T_MUL
    } tok_type;

    //the actual token structure
    struct token {
        std::string value;
        tok_type type;
        int line;
        int col;
    };

    extern int current_token_pos;
    extern token current_token;

    //functions
    void init(char* code);
    void peek();
    tok_type get_token();
    std::string type2str(tok_type type);
    void advance();
    std::vector<token> lex();
}