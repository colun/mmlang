# atcoder: https://abc153.contest.atcoder.jp/tasks/abc153_d
H = inputInt64()
@memo
def f(x:int64)->int64:
    if x==1:
        return 1
    return f(x/2) * 2 + 1
print(f(H))
