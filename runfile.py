from lex import Lexer
from par import Parser

file = open('main.asko', 'r').read()

l = Lexer(file)
p = Parser()

print("Tokens>")
t = l.Lex()
print(t)
print("Output>")
p.Parse(t)