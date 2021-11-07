# atcoder: abc172_d
N = inputInt()
sum = 0LL
for i in range(1, N + 1):
    sum += (i:N + 1:i)
print(sum)
