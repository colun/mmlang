N = inputInt()
c = inputWord()
R = 0
R += c[:N] == 'R'
W = 0
W += c[:N] == 'W'
assert R + W == N
sum = 0
sum += c[:R]=='W'
print(sum)
