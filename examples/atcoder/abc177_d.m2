# atcoder: abc177_d
N, M = inputInt(), inputInt()
uf = unionfind(N)
A[:M], B[$1] = inputInt() - 1, inputInt() - 1
uf.unite(A[:], B[$1])
C[:N] = uf.root($1)
C.sort()
before = -1
cnt = 0
ret = 0
for c in C:
    if c != before:
        before = c
        cnt = 1
    else:
        ++cnt
    ret >?= cnt
print(ret)
