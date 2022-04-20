# atcoder: https://atcoder.jp/contests/practice/tasks/practice_1

X: set!(int)

X.insert(3)
X.insert(7)

k_sum = 0
for k in X:
    k_sum += k

assert k_sum==10

a = inputInt()
b, c = inputInt(), inputInt()
s = inputWord()
print(a+b+c, s)
