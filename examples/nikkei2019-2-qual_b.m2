# atcoder: https://nikkei2019-2-qual.contest.atcoder.jp/tasks/nikkei2019_2_qual_b
N = inputInt()
D[:N] = inputInt()
S[:N] = 0
S[D[0:N]] += 1
if D[0] or S[0]!=1:
    print(0)
else:
    ret = int64(1)
    if S[i@1:N]:
        for _ in S[i]:
            ret = (ret * S[i-1]) % 998244353
    print(ret)
