N = inputInt()
A[:N] = inputInt()
B[:1e3 + 10] = 0
P[:0] = 0
if B[p@2:1001] == 0:
    P.push_back(p)
    B[p:1001:p] = 1
B[:1e6 + 10] = 0
pc = True
ret = A[0]
for a in A:
    ret = gcd(ret, a)
    b = a
    if b % (p@: P) == 0:
        while b % p == 0:
            b /= p
        if B[p]:
            pc = False
        ++B[p]
    if b != 1:
        if B[b]:
            pc = False
        ++B[b]
print(pc ? "pairwise coprime": ret == 1 ? "setwise coprime": "not coprime")
