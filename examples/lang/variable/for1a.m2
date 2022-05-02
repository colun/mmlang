# atcoder: https://atcoder.jp/contests/practice/tasks/practice_1

I[:3] = $1, $1+1, $1*$1

for i, j, k in I:
    if i==1:
        j += 2
        assert j==4

a = inputInt()
b, c = inputInt(), inputInt()
s = inputWord()
print(a+b+c, s)
