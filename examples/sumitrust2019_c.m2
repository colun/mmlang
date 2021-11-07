# atcoder: https://sumitrust2019.contest.atcoder.jp/tasks/sumitb2019_c
X = inputInt()

@memo
def f(x)->bool:
    if x==0:
        return True
    if x<100:
        return False
    if x<=105:
        return True
    return f(x-100) or f(x-101) or f(x-102) or f(x-103) or f(x-104) or f(x-105)

print(f(X) ? "1" : "0")
