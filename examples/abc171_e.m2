# atcoder: abc171_e
N = inputInt()
a[:N] = inputInt()
x = 0
x ^= a[:N]
print0(x ^ a[:N-1], "")
print(x ^ a[N-1])
