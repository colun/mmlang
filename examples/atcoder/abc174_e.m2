# atcoder: abc174_e
N, K = inputInt(), inputInt()
A[:N] = inputInt()
ans = 0
for b in range(31):
    ans2 = ans + (1<<(30-b))
    sum = 0
    sum += (A[:]-1) / ans2
    if K<sum:
        ans = ans2
print(ans+1)
