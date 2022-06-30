#include <parser/parse/statement.h>
#include <parser/parse/func.h>
#include <parser/parse/var.h>

namespace parser {
    using namespace lexer;

    void parse_statement(std::vector<parser::node>* ast) {
        std::string type=current_token.value;

        eat(tok_type::T_ID);

        tok_type t=get_token();
        if (t==tok_type::T_ID) {
            std::string name=current_token.value;

            eat(tok_type::T_ID);
            //id id

            switch (get_token()) {
                case T_LPAR:
                    //id id (
                    parse_funcdef(type, name, ast);
                    break;
                case T_EQ:
                    //id id =
                    parse_vardef(type, name, ast);
                    break;
                default:
                    error("Unexpected token '%s'.",
                            type2str(current_token.type).data());
                    break;
            }
        } else if (t==tok_type::T_LPAR) {
            //id (
            parse_funccall(type, ast);
        } else if (t==tok_type::T_SEMI) {
            //id ;
            parse_funccall(type, ast);
        } else {
            error("Unexpected token '%s'.",
                    type2str(current_token.type).data());
        }
    }
}