#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <codegen/gen.h>

int main(int argc, char** argv) {
    if (argc<=1) {
        printf("Usage: %s <path to file>\n", argv[0]);
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        if (size>0) {
            lexer::init(buffer.data());
        } else {
            printf("Info: Blank file, not going to compile.\n");
            exit(0);
        }
        lexer::lex();

        std::vector<parser::node> AST = parser::parse();

        gen::init(AST);
        std::string o=gen::run();

        std::ofstream file_out;

        file_out.open("askour_file_out_temp.asm");
        if (!file_out.is_open()) {
            printf("Error: Could not open assembly file.\n");
            exit(EXIT_FAILURE);
        }
        file_out << o;
        file_out.close();
    } else {
        printf("Error: Could not open file '%s'.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    file.close();

    return 0;
}