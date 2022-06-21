#include <parser/parser.h>
#include <stdarg.h>

namespace parser {
    using namespace lexer;
    
    void parse_statement(std::vector<node>* ast);
    
    void error(std::string fmt, ...) {
        va_list args;
        std::string buffer;
        va_start(args, fmt);
        vsprintf((char*)buffer.data(), fmt.c_str(), args);
        va_end(args);
        printf("[Parser]: %s\n", buffer.data());
        exit(EXIT_FAILURE);
    }

    token eat(tok_type token) {
        if (get_token() != token) {
            printf("Error: Expected token '%s' but got '%s' at %d:%d.\n",
                   type2str(token).data(),
                   type2str(current_token.type).data(),
                   current_token.line,
                   current_token.col);
            exit(EXIT_FAILURE);
        }
        advance();
        return current_token;
    }

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
                    val v={.data=current_token.value,.type=current_token.type};
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

    void parse_vdef(std::string type, std::string name, std::vector<node>* ast) {
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

    void parse_fdef(std::string type, std::string name, std::vector<node>* ast) {
        std::map<std::string, std::string> args=parse_arguments(ast);
        std::vector<node> body=parse_block(ast);

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

    void parse_fcall(std::string name, std::vector<node>* ast) {
        std::vector<val> args=parse_call_args(ast);

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

    void parse_vmod(std::string name, std::vector<node>* ast) {
        node n={
            .type=N_var_modify,
            .line=current_token.line,
            .col=current_token.col,
            .left={
                .data=name,
                .type=tok_type::T_ID
            },
            .value={
                .data=current_token.value,
                .type=current_token.type
            },
            .ptr=false
        };

        eat(current_token.type);

        eat(T_SEMI);

        ast->push_back(n);
    }

    void parse_vmath(std::string name, std::vector<node>* ast) {
        node n={
            .type=N_var_modify,
            .line=current_token.line,
            .col=current_token.col,
            .left={
                .data=name,
                .type=tok_type::T_ID
            },
            .value={
                .data=current_token.value,
                .type=current_token.type
            },
            .ptr=false
        };

        eat(current_token.type);

        eat(T_SEMI);

        ast->push_back(n);
    }

    void parse_statement(std::vector<node>* ast) {
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
                    parse_fdef(type, name, ast);
                    break;
                case T_EQ:
                    //id id =
                    parse_vdef(type, name, ast);
                    break;
                default:
                    error("Unexpected token '%s' at %d:%d.",
                          type2str(current_token.type).data(),
                          current_token.line,
                          current_token.col);
                    break;
            }
        } else if (t==tok_type::T_LPAR) {
            //id (
            parse_fcall(type, ast);
        } else if (t==tok_type::T_SEMI) {
            //id ;
            switch (current_token.type) {
                case T_SEMI:
                    //id ;
                    parse_fcall(type, ast);
                    break;
            }
        } else if (t==tok_type::T_EQ) {
            eat(T_EQ);
            //id =
            if (current_token.type==T_NUM || current_token.type==T_STR || current_token.type==T_ID) {
                //id = value
                parse_vmod(type, ast);
            } else {
                error("Unexpected token '%s' at %d:%d.",
                      type2str(current_token.type).data(),
                      current_token.line,
                      current_token.col);
            }
        } else if (t==tok_type::T_ADDEQ) {
            eat(T_ADDEQ);
            if (current_token.type==T_NUM || current_token.type==T_ID) {
                //id = value
                parse_vmath(type, ast);
            } else {
                error("Unexpected token '%s' at %d:%d.",
                      type2str(current_token.type).data(),
                      current_token.line,
                      current_token.col);
            }
        } else {
            error("Unexpected token '%s' at %d:%d.",
                  type2str(current_token.type).data(),
                  current_token.line,
                  current_token.col);
        }
    }

    std::vector<node> parse() {
        std::vector<node> ast;
        if (current_token.type==T_EOF) {
            ast.push_back((node){.type=node_type::N_EOF});
            return ast;
        }
        while (current_token.type != tok_type::T_EOF) {
            parse_statement(&ast);
        }
        ast.push_back((node){.type=node_type::N_EOF});
        return ast;
    }
}