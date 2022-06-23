CC=g++

SOURCES=$(wildcard ./src/*.cpp 				\
				   ./src/lexer/*.cpp		\
				   ./src/parser/*.cpp		\
				   ./src/parser/parse/*.cpp	\
				   ./src/codegen/*.cpp		\
				   ./src/codegen/gens/*.cpp	)

all: build run

build:
	@echo Building...
	@$(CC) -o out/asko $(SOURCES) -I./src/ -fpermissive

run:
	@echo Running...
	@./out/asko ./src/tests/main.asko

build_and_run_asm:
	@echo Building asm...
	@echo Warning! This uses WSL to compile the assembly file and link it
	@C:\Windows\sysnative\wsl.exe as askour_file_out_temp.asm -o asko_file.o
	@C:\Windows\sysnative\wsl.exe nasm -felf64 print.asm -o print.o
	@C:\Windows\sysnative\wsl.exe ld asko_file.o print.o -o out/program --entry=main
	@echo Running asm...
	@C:\Windows\sysnative\wsl.exe ./out/program