N, D = inputInt(), inputInt64()
X[:N], Y[$1] = inputInt64(), inputInt64()
sum = 0
sum += X[:N]*X[$1]+Y[$1]*Y[$1]<=D*D
print(sum)
