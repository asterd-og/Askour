tokens = []
symbols= {}

def Lex(file):
	comment      = ""
	token  		 = ""
	isstr  		 = 0
	iexpr        = 0
	iscm         = 0
	vst          = 0
	var          = ""
	string 		 = ""
	expr         = ""
	num          = ""
	filecontents = list(file)
	for c in filecontents:
		token += c
		#ignore spaces
		if token == " ":
			if isstr == 0:
				token = ""
			else:
				token = " "
		#ignore newline
		elif token == "\n" or token == "<EOF>":
			if expr != "" and iexpr == 1:
				tokens.append(f"expr:{expr}")
				expr = ""
			elif expr != "" and iexpr == 0:
				tokens.append(f"num:{expr}")
				expr = ""
			elif var != "":
				tokens.append(f"var:{var}")
				var = ""
				vst = 0
			token = ""
		#variable
		elif token == "=" and isstr == 0:
			if expr != "" and iexpr == 0:
				tokens.append(f"num:{expr}")
				expr = ""
			if var != "":
				tokens.append(f"var:{var}")
				var = ""
				vst = 0
			if tokens[-1] == "equals":
				tokens[-1] = "eqeq"
			else:
				tokens.append("equals")
			token = ""
		elif token == "@" and isstr == 0:
			vst = 1
			var += token
			token = ""
		elif vst == 1:
			if token == "<" or token == ">":
				if var != "":
					tokens.append(f"var:{var}")
					var = ""
					vst = 0
			var += token
			token = ""
		#print
		elif token.lower() == "print":
			tokens.append("print")
			token = ""
		#input
		elif token.lower() == "input":
			tokens.append("input")
			token = ""
		#end the if
		elif token == "endif":
			tokens.append("endif")
			token = ""
		#clear
		elif token == "clear":
			tokens.append("clear")
			token = ""
		#if
		elif token.lower() == "if":
			tokens.append("if")
			token = ""
		#then
		elif token == "then":
			if expr != "" and iexpr == 0:
				tokens.append(f"num:{expr}")
				expr = ""
			tokens.append("then")
			token = ""
		#comments
		elif token == "$":
			if iscm == 0:
				iscm = 1
			elif iscm == 1:
				comment += token
				tokens.append(f"comment:{comment}")
				comment = ""
				iscm = 0
				token = ""
		#numbers
		elif token in "0123456789":
			expr += token
			token = ""
		#expressions
		elif token in "+/*-()":
			iexpr = 1
			expr += token
			token = ""
		#ignore tabs
		elif token == "\t":
			token = ""
		#string
		elif token == "\"" or token == " \"":
			if isstr == 0:
				isstr = 1
			elif isstr == 1:
				tokens.append(f"string:{string}\"")
				string = ""
				isstr = 0
				token = ""
		#string
		elif isstr == 1:
			string += token
			token = ""
		#comment
		elif iscm == 1:
			comment += token
			token = ""
	# print(tokens)
	# return ''
	return tokens