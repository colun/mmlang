# atcoder: https://abc129.contest.atcoder.jp/tasks/abc129_c
N, M = inputInt(), inputInt()
a[:M] = inputInt()
b[:N+1] = 1
b[a[:M]] = 0
p[:N+1] = 0
p[0] = 1
if b[1:N+1]:
    if 2<=$1:
        p[$1] = (p[$1-1] + p[$1-2]) % 1000000007
    else:
        p[$1] = p[$1-1]
print(p[N])
