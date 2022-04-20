# atcoder: https://abc149.contest.atcoder.jp/tasks/abc149_d
N, K, R, S, P, T = inputInt(), inputInt(), inputInt(), inputInt(), inputInt(), inputWord()
sc = 0
for k in K:
    B[:3] = 0
    for n in range(k, N, K):
        A[:3] = max(B[($1+1)%3], B[($1+2)%3])
        if T[n]=='r':
            A[0] += P
        if T[n]=='s':
            A[1] += R
        if T[n]=='p':
            A[2] += S
        B[:3] = A[$1]
    sc += max(B[:])
print(sc)
