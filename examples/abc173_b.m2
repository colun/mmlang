# atcoder: abc173_b
N = inputInt()
S[:N] = inputWord()
W = ["AC", "WA", "TLE", "RE"]
revW[W[:]] = $1
C[:W.size()] = 0
++C[revW[S[:]]]
print(W[:], "x", C[$1])
