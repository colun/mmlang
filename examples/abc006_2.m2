# atcoder: https://abc006.contest.atcoder.jp/tasks/abc006_2
n = inputInt()
a[:n+5] = 0
a[3] = 1
a[4:n+1] = ((a[$1-1] + a[$1-2]) % 10007 + a[$1-3]) % 10007
print(a[n])
