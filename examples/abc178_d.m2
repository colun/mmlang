# atcoder: https://abc178.contest.atcoder.jp/tasks/abc178_d
S = inputInt()
ans = 0
for L in range(1, S / 3 + 1):
    remain = S - L*3
    F[:remain+1] = modint!(1000000007)(0)
    F[remain] = 1
    for l in L:
        s = 0
        for j in rrange(remain+1):
            s += F[j]
            F[j] = s
    ans += F[0]
print(ans)
