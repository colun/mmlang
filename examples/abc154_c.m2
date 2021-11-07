# atcoder: https://abc154.contest.atcoder.jp/tasks/abc154_c
N = inputInt()
A[:N] = inputInt()
A.sort()
print(sum(A[:N-1]==A[$1+1])==0 ? "YES" : "NO")
