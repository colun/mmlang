X, K, D = abs(inputInt128()), inputInt128(), inputInt128()
if K<=X/D:
    print(X-K*D)
else:
    M = X % D
    U = (K - X/D)
    print(U%2==0 ? M : D-M)
