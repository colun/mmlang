# atcoder: abc175_e
R, C, K = inputInt(), inputInt(), inputInt()
rcv[:K] = inputInt() - 1, inputInt() - 1, inputInt()
rcv.sort()
D[:R][:C][:4] = 0LL
for r in range(R):
    for c in range(C):
        v = 0
        v = (: rcv.where(r, c))[2]
        for i in range(4):
            if 1 <= i:
                D[r][c][i] >?= v
            if 1 <= r:
                if 1 <= i:
                    D[r][c][i] >?= D[r - 1][c][:4] + v
                else:
                    D[r][c][i] >?= D[r - 1][c][:4]
            if 1 <= c:
                D[r][c][i] >?= D[r][c - 1][:i] + v
                D[r][c][i] >?= D[r][c - 1][:i + 1]
print(D[R - 1][C - 1][3])
