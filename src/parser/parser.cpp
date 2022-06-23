#include <parser/parser.h>
#include <parser/parse/statement.h>
#include <stdarg.h>

namespace parser {
    using namespace lexer;
    
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