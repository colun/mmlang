N = inputInt()
A[:N + 1] = 0
for i in range(1, N + 1):
    ++A[i:N + 1:i]
sum = 0LL
sum += int64(A[:]) * $1
print(sum)
