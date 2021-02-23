S = inputWord()
T = inputWord()
ret = True
ret &= S[:]==T[$1]
print(ret ? "Yes" : "No")
