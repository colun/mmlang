N, Q = inputInt(), inputInt()
c[:N] = inputInt() - 1
LR[:Q] = inputInt() - 1, inputInt() - 1, $1
LR.rsort()
D[:N] = 0
ans[:Q] = 0
bit, n = BIT!(int)(N), N - 1
for l, r, q in LR:
    while l <= n:
        bit[D[c[n]]] = 0
        bit[D[c[n]] = n] = 1
        --n
    ans[q] = bit.sum(l, r + 1)
print(ans[:Q])
