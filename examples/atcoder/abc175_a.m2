# atcoder: abc175_a
S = inputWord()
r = 0
ret = 0
for c in S:
    if c=='R':
        ++r
    else:
        r = 0
    ret = max(ret, r)
print(ret)
