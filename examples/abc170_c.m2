# atcoder: abc170_c
X, N = inputInt(), inputInt()
p[:N] = inputInt()
D[:110] = 0
D[p[:N]+5] = 1
for i in range(200):
    if D[X-i+5]==0:
        print(X-i)
        break
    if D[X+i+5]==0:
        print(X+i)
        break
