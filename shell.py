from lexShell import Lexer
from par import Parser

l = Lexer()
p = Parser()

while True:
	i = input('>> ')
	t = l.Lex(i)
	p.Parse(t)