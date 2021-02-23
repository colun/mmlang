N = inputInt()
A[:N], B[$1] = inputInt(), inputInt()
A.sort()
B.sort()
if N % 2 == 1:
    print(B[N / 2] - A[N / 2] + 1)
else:
    print(B[N / 2 - 1] + B[N / 2] - A[N / 2 - 1] - A[N / 2] + 1)
