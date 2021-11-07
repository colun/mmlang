# atcoder: https://agc017.contest.atcoder.jp/tasks/agc017_a
N, P = inputInt(), inputInt()
A[:N] = inputInt() % 2
B[:2] = int64(1-$1)
for a in A:
    if a:
        C = sum(B[:])
        B[:] = C
    else:
        B[:] = B[$1] + B[$1]
print(B[P])
