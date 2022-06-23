#include <parser/parse/var.h>

namespace parser {
    using namespace lexer;

    void parse_vardef(std::string type, std::string name, std::vector<node>* ast) {
        eat(tok_type::T_EQ);

        node n={
            .type=N_var_def,
            .line=current_token.line,
            .col=current_token.col,
            .left={
                .data=type,
                .type=tok_type::T_ID
            },
            .middle={
                .data=name,
                .type=tok_type::T_ID,
            },
            .value={
                .data=current_token.value,
                .type=current_token.type
            }
        };


        eat(current_token.type); //value

        ast->push_back(n);

        eat(T_SEMI);
    }
}