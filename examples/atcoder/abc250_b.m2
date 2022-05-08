# atcoder: abc250_b ( https://atcoder.jp/contests/abc250/tasks/abc250_b )
N, A, B = inputInt(), inputInt(), inputInt()
for n in range(N):
    for a in range(A):
        for n2 in range(N):
            for b in range(B):
                print0((n+n2)&1 ? '#' : '.')
        print()
