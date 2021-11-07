# atcoder: https://abc214.contest.atcoder.jp/tasks/abc214_c
N = inputInt()
S[:N] = inputInt64()
T[:N] = inputInt64()
A[:1] = T[$1]
A[1:N] = min(A[$1-1]+S[$1-1], T[$1])
A[0] = min(A[N-1]+S[N-1], T[0])
A[1:N] = min(A[$1-1]+S[$1-1], T[$1])
print(A[:])
