# atcoder: abc177_c
N = inputInt()
A[:N] = inputInt()
sum = 0
sum += A[:] mod 1e9+7
ret = 0
for i in A.size():
    sum -= A[i] mod 1e9+7
    b = sum
    b *= A[i] mod 1e9+7
    ret += b mod 1e9+7
print(ret)
