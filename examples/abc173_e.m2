N, K = inputInt(), inputInt()
P = vector!(int)()
M = vector!(int)()
for i in range(N):
    if 0 < (a@inputInt()):
        P.emplace_back(a)
    else:
        M.emplace_back(a)
P.sort()
M.rsort()
ret = 1
if P.empty() and K % 2 == 1:
    ret *= M[:K] mod 1e9 + 7
elif N==K:
    ret *= M[:] mod 1e9 + 7
    ret *= P[:] mod 1e9 + 7
else:
    if K % 2 == 1:
        ret = P.pop() mod 1e9 + 7
        --K
    while K:
        if M.size() < 2:
            ret *= P.pop() mod 1e9 + 7
            ret *= P.pop() mod 1e9 + 7
        elif P.size() < 2:
            ret *= -M.pop() mod 1e9 + 7
            ret *= -M.pop() mod 1e9 + 7
        elif int64(M[M.size() - 1]) * M[M.size() - 2] < int64(P[P.size() - 1]) * P[P.size() - 2]:
            ret *= P.pop() mod 1e9 + 7
            ret *= P.pop() mod 1e9 + 7
        else:
            ret *= -M.pop() mod 1e9 + 7
            ret *= -M.pop() mod 1e9 + 7
        K -= 2
print(ret)
