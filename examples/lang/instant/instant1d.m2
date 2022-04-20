# atcoder: https://atcoder.jp/contests/practice/tasks/practice_1
a = inputInt()
b, c = inputInt(), inputInt()
s = inputWord()

vec: vector!(tuple!(float, int))
vec.emplace_back(s@sum(float(i)/((:10)+1)), s+(i@(:10)+1))
ans = [2.93, 5.86, 8.79, 11.72, 14.64, 17.57, 20.50, 23.43, 26.36, 29.29]
assert fabs(vec[:][0]-ans[$1]) < 0.01
print(a+b+c, s)
