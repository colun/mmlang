# atcoder: https://abc178.contest.atcoder.jp/tasks/abc178_c
N = inputInt()
p[:3] = int64(1)
for i in range(N):
    p[:3] = (p[$1] * (10-$1)) % 1000000007
ret = p[0] % 1000000007
ret = (ret + 1000000007 - p[1]) % 1000000007
ret = (ret + 1000000007 - p[1]) % 1000000007
ret = (ret + p[2]) % 1000000007
print(ret)
