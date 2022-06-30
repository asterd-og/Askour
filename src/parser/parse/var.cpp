#include <parser/parse/var.h>
#include <parser/parse/expression.h>

namespace parser {
    using namespace lexer;

    void parse_factor() {
    }

    void parse_vardef(std::string type, std::string name, std::vector<node>* ast) {
        eat(tok_type::T_EQ);

        std::vector<expr> _expr = parse_expression();

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
            ._expr=_expr
        };

        ast->push_back(n);
    }
}