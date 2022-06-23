#include <parser/parse/block.h>
#include <parser/parse/statement.h>

namespace parser {
    using namespace lexer;

    std::vector<node> parse_block(std::vector<node>* ast) {
        eat(tok_type::T_LBRACE);
        std::vector<node> block;
        int open=1;
        while (open>=1) {
            if (current_token.type==tok_type::T_EOF) {
                error("Unexpected end of file.");
            }
            if (current_token.type==tok_type::T_LBRACE) {
                open++;
                eat(tok_type::T_LBRACE);
            }
            else if (current_token.type==tok_type::T_RBRACE) {
                open--;
                eat(tok_type::T_RBRACE);
            } else {
                parse_statement(&block);
            }
        }
        return block;
    }
}