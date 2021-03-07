from lex import *
from par import *
from sys import *

def openfile(filename):
	data = open(filename, "r").read()
	data += "<EOF>"
	return data

def isopenfile():
	if(argv[1]):
		return True
	else:
		return False

if isopenfile():
	d = openfile(argv[1])
	t = Lex(d)
	p = parse(t)