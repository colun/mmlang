# atcoder: https://abc208.contest.atcoder.jp/tasks/abc208_b
P = inputInt()
A[:2] = 1
A[1:11] = A[$1-1] * $1
A.reverse()
C = 0
if A[:10]:
    C += P / A[$1]
    P -= P / A[$1] * A[$1]
print(C)
