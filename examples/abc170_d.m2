N = inputInt()
A[:N] = inputInt()
D[:1e6 + 8] = 0
++D[A[:N]]
A.sort()
ret = 0
for a in A:
    --D[a]
    for k in range(1, 1001):
        if a % k == 0:
            if D[k]:
                break
            elif D[a/k]:
                break
    else:
        ++ret
    ++D[a]
print(ret)
