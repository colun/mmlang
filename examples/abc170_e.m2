# atcoder: abc170_e
N, Q = inputInt(), inputInt()
A[:N], B[$1] = inputInt(), inputInt()-1
CD[:Q] = inputInt()-1, inputInt()-1
S[:2e5+8] = set()
S[B[:]] += A[$1], $1
R = set()
if S[s@:]:
    R += S[s].max()
for C, D in CD:
    if S[B[C]]:
        R -= S[B[C]].max()
    if S[D]:
        R -= S[D].max()
    S[B[C]] -= A[C], C
    S[D] += A[C], C
    if S[B[C]]:
        R += S[B[C]].max()
    if S[D]:
        R += S[D].max()
    B[C] = D
    print(R.min()[0])
