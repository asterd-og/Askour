CC=g++

SOURCES=$(wildcard ./src/*.cpp 				\
				   ./src/lexer/*.cpp		\
				   ./src/parser/*.cpp		\
				   ./src/codegen/*.cpp		\
				   ./src/codegen/gens/*.cpp	)

all: build run

build:
	@echo Building...
	@$(CC) -o out/asko $(SOURCES) -I./src/ -fpermissive

run:
	@echo Running...
	@./out/asko ./src/tests/main.asko