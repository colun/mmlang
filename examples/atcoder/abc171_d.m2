# atcoder: abc171_d
N = inputInt()
A[:N] = inputInt()
Q = inputInt()
BC[:Q] = inputInt(), inputInt()
S = 0LL
S += A[:N]
cnt = vector!(int)(1e5+8)
++cnt[A[:N]]
for B, C in BC:
    S += (C-B) * cnt[B]
    cnt[C] += cnt[B]
    cnt[B] = 0
    print(S)
