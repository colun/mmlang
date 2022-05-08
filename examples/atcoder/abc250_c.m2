# atcoder: abc250_c ( https://atcoder.jp/contests/abc250/tasks/abc250_c )
N, Q = inputInt(), inputInt()
R[:N] = $1
D[:N] = 0
D[R[:N]] = $1
for j in range(Q):
    x = inputInt() - 1
    idx = min(D[x], N-2)
    v = R[idx]
    R[idx] = R[idx+1]
    R[idx+1] = v
    D[R[idx]] = idx
    D[R[idx+1]] = idx+1
for i in range(N):
    if i:
        print0(' ')
    print0(R[i]+1)
print()
