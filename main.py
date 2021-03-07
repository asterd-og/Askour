from lex import *
from par import *
def open_file(filename):
	data = open(filename, "r").read()
	data += "<EOF>"
	return data
d = open_file('main.asko')
t = Lex(d)
p = parse(t)