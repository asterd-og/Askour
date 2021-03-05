class Lexer:
	def __init__(self):
		self.token = ""
		self.isstr = 0
		self.cstring = ""

	def Lex(self, filecontents):
		token = self.token
		string = self.cstring
		tokens = []
		isstr = self.isstr

		fcont = list(filecontents)

		for c in fcont:
			token += c
			if token == " ":
				if isstr == 0:
					token = ""
				else:
					token = " " #ignore spaces, but if is a string, then NO
			elif token == "\n": #ignore new line
				token = ""
			elif token.lower() == "print": #print command
				tokens.append("Print") #append the command to the "self.tokens" list, so we can make the parser with this self.tokens list
				token = "" #reset current token (~~current line~~)
			elif token == "\"": #the \" is to the " not bug... noww we gonna make strings
				if isstr == 0: #if is a string equals to zero then...
					isstr = 1
				elif isstr == 1:
					tokens.append(f"String {string}\"")
					string = ""
					isstr = 0
					token = ""
			elif isstr == 1:
				string += token
				token = ""
		return tokens
