# atcoder: https://abc125.contest.atcoder.jp/tasks/abc125_d
N = inputInt()
A[:N] = inputInt()
P[:2] = int64(0)
P[0] = 0
P[1] = -2000000000
for a in A:
    B[:2] = max(P[$1]+a, P[($1+1)%2]-a)
    P[:2] = B[$1]
print(P[0])
