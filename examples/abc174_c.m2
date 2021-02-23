K = inputInt()
X = 7 mod K
for i in range(K):
    if X==0:
        print(i+1)
        break
    X *= 10 mod K
    X += 7 mod K
else:
    print(-1)
