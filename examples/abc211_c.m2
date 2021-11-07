# atcoder: https://abc211.contest.atcoder.jp/tasks/abc211_c
S = inputWord()
DP[:9] = 0
DP[0] = 1
chokudai = "chokudai"
for s in S:
    if chokudai[:]==s:
        DP[$1+1] = (DP[$1] + DP[$1+1]) % 1000000007
print(DP[8])
