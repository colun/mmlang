# atcoder: https://hhkb2020.contest.atcoder.jp/tasks/hhkb2020_c
N = inputInt()
p[:N] = inputInt()
a:BIT!(int)
a.resize(262144)
for i in p:
    if a[i]==0:
        a[i] += 1
    x = 0
    y = 131072
    while y:
        if a.sum(x+y)==x+y:
            x += y
        y /= 2
    print(x)
