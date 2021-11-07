# atcoder: abc168_d
N, M = inputInt(), inputInt()
AB[:M] = inputInt() - 1, inputInt() - 1
AB[M:M * 2] = AB[$1 - M][1], AB[$1 - M][0]
AB.sort()
C[:N] = -1
C[0] = 0
F = queue!(int)()
F.push(0)
for f in F:
    for ab in AB.where(f):
        if C[t@ab[1]] == -1:
            C[t] = C[f] + 1
            F.push(t)
print("Yes")
for f in range(1, N):
    V[:0] = 0, 0
    V.clear()
    for ab in AB.where(f):
        V.emplace_back(C[t@ab[1]], t)
    V.sort()
    print(V[0][1] + 1)
