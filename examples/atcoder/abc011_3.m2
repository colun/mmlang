# atcoder: https://abc011.contest.atcoder.jp/tasks/abc011_3
N = inputInt()
NG[:3] = inputInt()
DP[:N+5] = 10000
DP[N] = 0
for n in rrange(N):
    DP[NG[:3]] = 10000
    DP[n] = min(DP[n+(1:4)]) + 1
print(DP[0]<=100 ? "YES" : "NO")
