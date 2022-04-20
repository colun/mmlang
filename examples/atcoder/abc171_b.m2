# atcoder: abc171_b
N, K = inputInt(), inputInt()
p[:N] = inputInt()
p.sort()
sum = 0
sum += p[:K]
print(sum)
