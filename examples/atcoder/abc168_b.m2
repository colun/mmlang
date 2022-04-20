# atcoder: abc168_b
K = inputInt()
S = inputWord()
if S.size()<=K:
    print(S)
else:
    print0(S[:K])
    print("...")
