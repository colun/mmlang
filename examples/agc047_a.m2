N = inputInt()
B[:N] = 0, 0
for i in range(N):
    a, s = inputFraction64()
    B[i] = -s, -s
    while a % 2 == 0:
        a /= 2
        ++B[i][0]
    while a % 5 == 0:
        a /= 5
        ++B[i][1]
B.sort()
sum = 0LL
n = N - 1
bit = BIT!(int)(100)
for b2, b5 in B:
    assert -50<=b2<=50
    assert -50<=b5<=50
    while 0 <= n and 0 <= b2 + B[n][0]:
        ++bit[B[n][1] + 50]
        --n
    sum += bit.sum_back(50 - b5)
    sum -= 0 <= b2 and 0 <= b5
assert sum % 2 == 0
print(sum / 2)
