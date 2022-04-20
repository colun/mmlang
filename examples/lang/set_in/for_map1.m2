# atcoder: https://atcoder.jp/contests/practice/tasks/practice_1

X: map!(int, int)

X[3] = 2
X[7] = 5

k_sum, v_sum = 0, 0
for k in X:
    k_sum += k
    v_sum += X[k]

assert k_sum==10
assert v_sum==7

a = inputInt()
b, c = inputInt(), inputInt()
s = inputWord()
print(a+b+c, s)
