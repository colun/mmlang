N, Q = inputInt(), inputInt()
A[:N], B[$1] = inputInt(), inputInt()-1
CD[:Q] = inputInt()-1, inputInt()-1
S[:2e5+8] = multiset()
S[B[:]] += A[$1]
R = multiset()
if S[s@:]:
    R += max(S[s])
for C, D in CD:
    if S[B[C]]:
        R -= max(S[B[C]])
    if S[D]:
        R -= max(S[D])
    S[B[C]] -= A[C]
    S[D] += A[C]
    if S[B[C]]:
        R += max(S[B[C]])
    if S[D]:
        R += max(S[D])
    B[C] = D
    print(min(R))
