N = inputInt()
A[:N] = inputInt()
A.rsort()
ret = 0LL
ret += A[(1:N) >> 1]
print(ret)
