# atcoder: https://atcoder.jp/contests/practice/tasks/practice_1

x, y:int, z:string = "Hello", 3, "Bye"

D: fast_array!(int, y)

D[:3] = $1

assert x=="Hello"
assert y==3
assert z=="Bye"

assert D[:3]==$1

a = inputInt()
b, c = inputInt(), inputInt()
s = inputWord()
print(a+b+c, s)
