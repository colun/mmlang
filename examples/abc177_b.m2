# atcoder: abc177_b
S = inputWord()
T = inputWord()
ret = T.size()
for i in S.size() - T.size() + 1:
    c = 0
    c += T[:]!=S[i+$1]
    ret <?= c
print(ret)
