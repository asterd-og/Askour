from lex import symbols
import os

def evalexpr(teval):
	return eval(teval)

def prnt(tprt):
	if tprt[0:6] == "string":
		tprt = tprt[8:]
		tprt = tprt[:-1]
	elif tprt[0:3] == "num":
		tprt = tprt[4:]
	elif tprt[0:4] == "expr":
		tprt = evalexpr(tprt[5:])
	print(tprt)

def assign(varn, varv):
	symbols[varn[4:]] = varv

def gv(varn):
	varn = varn[4:]
	if varn in symbols:
		return symbols[varn]
	else:
		return f"Undefined Var \"{varn[1:]}\""
		exit()

def gi(st, varn):
	r = input(st[1:-1] + " ")
	symbols[varn] = f"string:\"{r}\""

def doclear():
	if os.name == "nt":
		os.system("cls")
	else:
		os.system("clear")

isif = 0

def parse(tokens):
	t = 0
	while t<len(tokens):
		#endif
		if tokens[t] == "endif" or tokens[t] == "clear":
			if tokens[t] == "endif":
				isif = 0
			elif tokens[t] == "clear":
				doclear()
			t+=1
		#print
		elif tokens[t] + " " + tokens[t+1][0:6] == "print string" or tokens[t] + " " + tokens[t+1][0:3] == "print num" or tokens[t] + " " + tokens[t+1][0:4] == "print expr" or tokens[t] + " " + tokens[t+1][0:3] == "print var":
			if tokens[t+1][0:6] == "string":
				prnt(tokens[t+1])
			elif tokens[t+1][0:3] == "num":
				prnt(tokens[t+1])
			elif tokens[t+1][0:4] == "expr":
				prnt(tokens[t+1])
			elif tokens[t+1][0:3] == "var":
				prnt(gv(tokens[t+1]))
			t+=2
		#variables
		elif tokens[t][0:3] + " " + tokens[t+1] + " " + tokens[t+2][0:6] == "var equals string" or tokens[t][0:3] + " " + tokens[t+1] + " " + tokens[t+2][0:3] == "var equals num" or tokens[t][0:3] + " " + tokens[t+1] + " " + tokens[t+2][0:4] == "var equals expr" or tokens[t][0:3] + " " + tokens[t+1] + " " + tokens[t+2][0:3] == "var equals var":
			if tokens[t+2][0:6] == "string":
				assign(tokens[t],tokens[t+2])
			elif tokens[t+2][0:3] == "num":
				assign(tokens[t],tokens[t+2])
			elif tokens[t+2][0:4] == "expr":
				assign(tokens[t],"num:" + str(evalexpr(tokens[t+2][5:])))
			elif tokens[t+2][0:3] == "var":
				assign(tokens[t],gv(tokens[t+2]))
			t+=3
		#input
		elif tokens[t] + " " + tokens[t+1][0:6] + " " + tokens[t+2][0:3] == "input string var":
			gi(tokens[t+1][7:],tokens[t+2][4:])
			t+=3
		#conditions
		elif tokens[t] + " " + tokens[t+1][0:3] + " " + tokens[t+2]  + " " + tokens[t+3][0:3] + " " + tokens[t+4] == "if num eqeq num then":
			isif = 1
			if tokens[t+1][4:] == tokens[t+3][4:]:
				pass
			else:
				while tokens[t] != "endif":
					if tokens == "endif":
						index = t
						tokens.pop(index)
					else:
						pass
			t+=5