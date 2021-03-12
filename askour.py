from lex import *
from par import *
import sys
import os
os.system('title Askour')
def openfile(filename):
	data = open(filename, "r").read()
	data += "<EOF>"
	return data
data = openfile(sys.argv[1])
l = Lex(data)
p = parse(l)