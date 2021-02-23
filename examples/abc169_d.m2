N = inputInt64()
ret = 0
for i in range(2, 1000001):
    j = int64(i)
    while N % j == 0:
        N /= j
        ++ret
        j *= i
    while N % i == 0:
        N /= i
if 2 <= N:
    ++ret
print(ret)
