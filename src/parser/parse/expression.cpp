#include <parser/parse/expression.h>

namespace parser {
    using namespace lexer;

    //WARNING:
    // - (Probably) bad code ahead :(
        // cause im not good with these things of expression parser.
        // sorry!
    
    //IDEA: 
    // - expr struct:
    // - - Type     (Tok type)
    // - - Left op  (Val)
    // - - Rigt op  (Val)
    // - [ADD, 5, 2]
    // - [ADD, 5, myvar], [ADD, 6, 9] -> 5+myvar + 6 + 9
    // - [ID, myvar] -> myvar

    // I didnt wanted to write a wall of code
    //but thats the only thing that i can think of right now (sadly)

    //but if it works, then ill be happy already

    std::vector<expr> parse_expression() {
        std::vector<expr> _expr_list;
        while (current_token.type != tok_type::T_SEMI) {
            if (current_token.type == tok_type::T_ID  ||
                current_token.type == tok_type::T_NUM ||
                current_token.type == tok_type::T_STR) {
                
                val left_op = (val){.data=current_token.value, .type=current_token.type};
                eat(current_token.type);

                if (current_token.type == tok_type::T_ADD ||
                    current_token.type == tok_type::T_SUB ||
                    current_token.type == tok_type::T_DIV ||
                    current_token.type == tok_type::T_MUL) {

                    tok_type mid_op = current_token.type;
                    eat(mid_op);

                    if (current_token.type == tok_type::T_ID ||
                        current_token.type == tok_type::T_NUM) {

                        //right operator.
                        
                        val right_op = (val){.data=current_token.value, .type=current_token.type};
                        eat(current_token.type);

                        expr _expr = (expr){
                            .type = mid_op,
                            .left = left_op,
                            .right = right_op
                        };

                        _expr_list.push_back(_expr);

                        if (current_token.type == tok_type::T_SEMI) {
                            eat(T_SEMI);
                            break;
                        }
                    } else {
                        error("Invalid right operator '%s'.", current_token.value.c_str());
                    }
                } else if (current_token.type == tok_type::T_SEMI ||
                           current_token.type == tok_type::T_NUM  ||
                           current_token.type == tok_type::T_STR) {
                    eat(T_SEMI);
                    expr _expr = (expr){
                        .type = left_op.type,
                        .left = left_op,
                        ._continue = false
                    };

                    _expr_list.push_back(_expr);
                    break;
                } else {
                    error("Invalid middle operator '%s'.", current_token.value.c_str());
                }
            } else if (current_token.type == tok_type::T_ADD ||
                       current_token.type == tok_type::T_SUB ||
                       current_token.type == tok_type::T_DIV ||
                       current_token.type == tok_type::T_MUL) {
                //5 + 3 **+** 2
                tok_type mid_op = current_token.type;
                eat(mid_op);

                if (current_token.type == tok_type::T_ID ||
                    current_token.type == tok_type::T_NUM) {

                    //right operator.
                    
                    val left_op = (val){.data=current_token.value, .type=current_token.type};
                    eat(current_token.type);

                    expr _expr = (expr){
                        .type = mid_op,
                        .left = left_op,
                        ._continue = true
                    };

                    _expr_list.push_back(_expr);

                    if (current_token.type == tok_type::T_SEMI) {
                        eat(T_SEMI);
                        break;
                    }
                } else {
                    error("Invalid right operator '%s'.", current_token.value.c_str());
                }
            } else {
                error("Invalid left operator '%s'.", current_token.value.c_str());
            }
        }
        return _expr_list;
    }
}