#include <lexer/lexer.h>
#include <stdarg.h>
#include <string.h>
#include <map>

namespace lexer {
    char  c;
    char* code;

    int line=2;
    int col=1;
    int pos;
    
    int current_token_pos=0;
    token current_token;

    std::vector<token> tokens;

    std::map<tok_type, std::string> toks2str={
        {T_EOF, "EOF"},
        {T_NL, "NL"},
        {T_NUM, "NUM"},
        {T_STR, "STR"},
        {T_ID, "ID"},
        {T_LPAR, "LPAR"},
        {T_RPAR, "RPAR"},
        {T_LBRACE, "LBRACE"},
        {T_RBRACE, "RBRACE"},
        {T_LSBRACE, "LSBRACE"},
        {T_RSBRACE, "RSBRACE"},
        {T_STAR, "STAR"},
        {T_SEMI, "SEMI"},
        {T_COMMA, "COMMA"},
        {T_EQEQ, "EQEQ"},
        {T_LTEQ, "LTEQ"},
        {T_LT, "LT"},
        {T_GTEQ, "GTEQ"},
        {T_GT, "GT"},
        {T_NEQ, "NEQ"},
        {T_EQ, "EQ"}
    };

    void init(char* l_code) {
        code=l_code;
    }

    void lex_error(std::string fmt, ...) {
        std::string buf;
        va_list va;
        va_start(va, fmt.data());
        int out=vsprintf((char*)buf.data(), fmt.data(), va);
        va_end(va);
        printf("[Lexer]: Error: %s", buf.data());
        exit(EXIT_FAILURE);
    }

    token add_token(std::string value, tok_type type) {
        token t={
            .value=value,
            .type=type,
            .line=line,
            .col=col
        };

        tokens.push_back(t);

        return t;
    }

    void peek() {
        if (c=='\n') {
            line++;
            col=1;
        } else if (c=='\r') {
            col=1;
        } else if (c==' ' || c=='\t') {
            col++;
        } else if (c>='a' && c<='z' ||
                   c>='A' && c<='Z' ||
                   c=='_') {
            //[a-zA-Z_][0-9]*
            std::string val;
            int tcol=col;
            while (c>='a' && c<='z' ||
                   c>='A' && c<='Z' ||
                   c=='_' || c>='0' && c<='9') {
                val.push_back(c);
                pos++;
                c=code[pos];
                tcol++;
            }
            pos--;
            col=tcol;
            add_token(val, tok_type::T_ID);
        } else if (c>='0' && c<='9') {
            //[0-9]+
            std::string val;
            int tcol=col;
            while (c>='0' && c<='9') {
                val.push_back(c);
                pos++;
                c=code[pos];
                tcol++;
            }
            pos--;
            col=tcol;
            add_token(val, tok_type::T_NUM);
        } else if (c=='"') {
            std::string val;
            pos++;
            c=code[pos];
            int tcol=col;
            while (c!='"') {
                if (c=='\n') {
                    lex_error("Expected token '%c' at %d:%d\n", '"', line, col);
                }
                val.push_back(c);
                pos++;
                c=code[pos];
                tcol++;
            }
            col=tcol;
            add_token(val, tok_type::T_STR);
        } else if (c=='\'') {
            std::string val;
            if (code[pos+1]!='\'') {
                if (code[pos]=='\\') {
                    //\r or \n or \b \t etc
                    val.push_back(c);
                    pos++;
                    c=code[pos];
                    val.push_back(c);
                    col++;
                    col++; //skip '
                    c=code[pos];
                } else {
                    lex_error("Expected token '%c' at %d:%d\n", '\'', line, col);
                }
            } else {
                val.push_back(c);
                pos++;
                c=code[pos];
                col++;
                col++; //skip '
            }
            add_token(val, tok_type::T_CHAR);
        } else if (c=='/') {
            if (code[pos+1]=='/') {
                //its a comment
                while (c!='\n') {
                    pos++;
                    c=code[pos];
                    col++;
                }
            } else {
                //its division operator!
                add_token("/", tok_type::T_DIV);
            }
        }
        //CONDITIONS
        else if (c=='=') {
            if (code[pos+1]=='=') {
                pos++; //go to the next =
                add_token("==", tok_type::T_EQEQ);
                col++;
                col++;
            } else {
                add_token("=", tok_type::T_EQ);
            }
        } else if (c=='<') {
            if (code[pos+1]=='=') {
                pos++; //go to the next =
                add_token("<=", tok_type::T_LTEQ);
                col++;
                col++;
            } else {
                add_token("<", tok_type::T_LT);
            }
        } else if (c=='>') {
            if (code[pos+1]=='=') {
                pos++; //go to the next =
                add_token(">=", tok_type::T_GTEQ);
                col++;
                col++;
            } else {
                add_token(">", tok_type::T_GT);
            }
        } else if (c=='!') {
            if (code[pos+1]=='=') {
                pos++; //go to the next =
                add_token("!=", tok_type::T_NEQ);
                col++;
                col++;
            } else {
                lex_error("Expected token '%c' at %d:%d\n", '=', line, col);
            }
        }
        //EXPRESSIONS
        else if (c=='+') {
            if (code[pos+1]=='=') {
                pos++;
                add_token("+=", tok_type::T_ADDEQ);
                col++;
                col++;
            } else {
                add_token("+", tok_type::T_ADD);
            }
        } else if (c=='-') {
            if (code[pos+1]=='=') {
                pos++;
                add_token("-=", tok_type::T_SUBEQ);
                col++;
                col++;
            } else {
                add_token("-", tok_type::T_SUB);
            }
        } else if (c=='*') {
            if (code[pos+1]=='=') {
                pos++;
                add_token("*=", tok_type::T_MULEQ);
                col++;
                col++;
            } else {
                add_token("*", tok_type::T_MUL);
            }
        } else {
            col++;
            switch (c) {
                case '(': add_token("(", tok_type::T_LPAR); break;
                case ')': add_token(")", tok_type::T_RPAR); break;
                case '[': add_token("[", tok_type::T_LSBRACE); break;
                case ']': add_token("]", tok_type::T_RSBRACE); break;
                case '{': add_token("{", tok_type::T_LBRACE); break;
                case '}': add_token("}", tok_type::T_RBRACE); break;
                case ';': add_token(";", tok_type::T_SEMI); break;
                case ',': add_token(",", tok_type::T_COMMA); break;
                case '*': add_token("*", tok_type::T_STAR); break;
                case ':': add_token(":", tok_type::T_COLON); break;
                default: lex_error("Unexpected token '%d' at %d:%d\n", (int)c, line, col);
            }
        }
    }
    
    std::vector<token> lex() {
        if (strlen(code)>0) {
            for (;pos<strlen(code);) {
                c=code[pos];
                peek();
                pos++;
            }
        }
        add_token("EOF", tok_type::T_EOF);
        get_token();
        return tokens;
    }

    tok_type get_token() {
        if (tokens.size()==0) {
            return;
        }
        current_token=tokens[current_token_pos];
        return current_token.type;
    }

    void advance() {
        current_token_pos++;
        current_token=tokens[current_token_pos];
    }

    std::string type2str(tok_type type) {
        return toks2str[type];
    }
}