# atcoder: abc167_b
A, B, C, K = inputInt(), inputInt(), inputInt(), inputInt()
a = min(A, K)
K -= a
b = min(B, K)
K -= b
c = min(C, K)
print(a - c)
