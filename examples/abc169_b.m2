N = inputInt()
A[:N] = inputInt128()
ZERO = 0
ZERO += A[:N] == 0
if ZERO:
    print(0)
    return
S = int128(1)
for a in A:
    S *= a
    if int128(1e18) < S:
        print(-1)
        return
print(S)
