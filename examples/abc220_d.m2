# atcoder: https://abc220.contest.atcoder.jp/tasks/abc220_d
N = inputInt()
P[:10] = 0
P[inputInt()] = 1
A[:N-1] = inputInt()
for i in A:
    Q[:10] = int64(0)
    Q[($1 * i) % 10] += P[:10]
    Q[($1 + i) % 10] += P[:10]
    P[:10] = Q[$1] % 998244353
print(P[:10])
