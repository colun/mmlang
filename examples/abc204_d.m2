# atcoder: https://abc204.contest.atcoder.jp/tasks/abc204_d
N = inputInt()
T[:N] = inputInt()
G[:110000] = 0
G[0] = 1
TOTAL = sum(T[:])
for t in T:
    if G[::-1]:
        G[$1+t] = 1
ret = TOTAL
if G[:]:
    ret = min(ret, max($1, TOTAL-$1))
print(ret)
