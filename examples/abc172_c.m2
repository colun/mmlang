# atcoder: abc172_c
N, M, K = inputInt(), inputInt(), inputInt64()
A[:N] = inputInt64()
B[:M] = inputInt64()
A[1:N] += A[$1 - 1]
B[1:M] += B[$1 - 1]
ret = B.upper_bound(K)
if A[:] <= K:
    ret = max(ret, $1 + 1 + B.upper_bound(K - A[$1]))
print(ret)
