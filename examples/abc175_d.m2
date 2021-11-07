# atcoder: abc175_d
N, K = inputInt(), inputInt()
P[:N] = inputInt()-1
C[:N] = inputInt()
ret = -100000000000LL
for start in range(N):
    sz = N
    i = start
    for j in range(N):
        i = P[i]
        if i==start:
            sz = j+1
            break
    K2 = K % sz
    CYCLE = K / sz
    MAX_SINGLE, MAX_CYCLE = -100000000000LL, -100000000000LL
    sum = 0LL
    for j in range(min(K, sz)):
        i = P[i]
        if i==start:
            sz = j
        sum += C[i]
        MAX_SINGLE = max(MAX_SINGLE, sum)
        if j<K2:
            MAX_CYCLE = max(MAX_CYCLE, sum)
    MAX_CYCLE += max(0LL, sum) * max(0, CYCLE)
    MAX_SINGLE += max(0LL, sum) * max(0, CYCLE-1)
    ret = max(ret, MAX_CYCLE, MAX_SINGLE)
print(ret)
