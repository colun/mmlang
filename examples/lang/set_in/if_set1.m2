# atcoder: https://atcoder.jp/contests/practice/tasks/practice_1

X: set!(int)

X.insert(3)
X.insert(7)

v = 0

if 3 in X:
    v += 1
if 5 in X:
    v += 2
if 7 in X:
    v += 4

assert v==5

a = inputInt()
b, c = inputInt(), inputInt()
s = inputWord()
print(a+b+c, s)
