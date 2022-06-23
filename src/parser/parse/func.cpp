#include <parser/parse/func.h>
#include <parser/parse/arguments.h>
#include <parser/parse/block.h>

namespace parser {
    using namespace lexer;

    void parse_funcdef(std::string type, std::string name, std::vector<node>* ast) {
        std::map<std::string, std::string> args = parse_arguments(ast);
        std::vector<node> body = parse_block(ast);
        
        node n={
            .type=N_func_def,
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
            .inner_nodes=body,
            .arguments=args
        };

        ast->push_back(n);
    }
    
    void parse_funccall(std::string name, std::vector<node>* ast) {
        std::vector<val> args = parse_call_args(ast);
        
        eat(T_SEMI);

        node n={
            .type=N_func_call,
            .line=current_token.line,
            .col=current_token.col,
            .left={
                .data=name,
                .type=tok_type::T_ID
            },
            .cargs=args
        };

        ast->push_back(n);
    }
}
