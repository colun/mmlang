# atcoder: abc173_c
H = inputInt()
W = inputInt()
K = inputInt()
c[:H] = inputWord()
ret = 0
for h in range(1 << H):
    for w in range(1 << W):
        k = 0
        k += c[:H][:W] == '#' and (h >>$1) & 1 and (w >>$2) & 1
        if k == K:
            ++ret
print(ret)
