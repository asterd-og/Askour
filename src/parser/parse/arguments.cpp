#include <parser/parse/arguments.h>
#include <lexer/lexer.h>

namespace parser {
    using namespace lexer;

    std::map<std::string, std::string> parse_arguments(std::vector<node>* ast) {
        std::map<std::string, std::string> args;
        if (get_token() == tok_type::T_LPAR) {
            eat(tok_type::T_LPAR);
            while (get_token() != tok_type::T_RPAR) {
                if (get_token() != tok_type::T_ID) {
                    error("Expected identifier but got '%s' at %d:%d.",
                          type2str(current_token.type).data(),
                          current_token.line,
                          current_token.col);
                }
                std::string name = current_token.value;
                eat(tok_type::T_ID);
                if (get_token() != tok_type::T_COLON) {
                    error("Expected ':' but got '%s' at %d:%d.",
                          type2str(current_token.type).data(),
                          current_token.line,
                          current_token.col);
                }
                eat(tok_type::T_COLON);
                if (get_token() != tok_type::T_ID) {
                    error("Expected identifier but got '%s' at %d:%d.",
                          type2str(current_token.type).data(),
                          current_token.line,
                          current_token.col);
                }
                std::string type = current_token.value;
                eat(tok_type::T_ID);
                args[name] = type;
                if (get_token() == tok_type::T_COMMA) {
                    eat(tok_type::T_COMMA);
                }
            }
            eat(tok_type::T_RPAR);
        }
        return args;
    }

    std::vector<val> parse_call_args(std::vector<node>* ast) {
        std::vector<val> args;
        if (get_token() == tok_type::T_LPAR) {
            eat(tok_type::T_LPAR);
            while (get_token() != tok_type::T_RPAR) {
                if (get_token() == tok_type::T_NUM || get_token() == tok_type::T_STR || get_token() == tok_type::T_ID) {
                    val v;
                    
                    v.data=current_token.value;
                    v.type=current_token.type;
                    args.push_back(v);
                    eat(get_token());
                } else {
                    error("Expected argument but got '%s' at %d:%d.",
                          type2str(current_token.type).data(),
                          current_token.line,
                          current_token.col);
                }
                if (get_token() != tok_type::T_RPAR) eat(tok_type::T_COMMA);
            }
            eat(tok_type::T_RPAR);
        }
        return args;
    }
}