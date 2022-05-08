# atcoder: agc057_a ( https://atcoder.jp/contests/agc057/tasks/agc057_a )
T = inputInt()
def func(x:int)->int:
    y = 0
    while x/10:
        x /= 10
        ++y
    for i in range(y):
        x *= 10
    return x

def func2(x:int)->int:
    y = 0
    while x/10:
        x /= 10
        ++y
    if x==1:
        return 0
    x = 1
    for i in range(y):
        x *= 10
    return x-1

for i in range(T):
    L, R = inputInt(), inputInt()
    print(R-max(R/10, L-1, R-func(R), func2(R)))
