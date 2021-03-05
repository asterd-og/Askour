class Parser:
	def __init__(self):
		pass

	def Parse(self, tokens):
		t = 0
		tokens = tokens
		while t < len(tokens):
			if tokens[t] + " " + tokens[t+1][0:6] == "Print String":
				if tokens[t+1][0:6] == "String":
					print(tokens[t+1][8:])
					t += 2