# atcoder: https://atcoder.jp/contests/practice/tasks/practice_1

I[:3] = $1, $1+1, $1*$1

for i, j, k in I:
    if i==1:
        j += k
        assert j==3

a = inputInt()
b, c = inputInt(), inputInt()
s = inputWord()
print(a+b+c, s)
