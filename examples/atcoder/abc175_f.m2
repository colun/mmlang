# atcoder: abc175_f
N = inputInt()
S[:N], C[$1] = inputWord(), inputInt()
R[:N] = S[$1]
R[:N].reverse()
@memo(cycle=1e18LL)
def func(overIsPrefix: bool, over: string) -> int64:
    ret = 1e18LL
    for n in N:
        ret <?= func2(over, overIsPrefix ? S[n] : R[n], overIsPrefix) + C[n]
    return ret
def func2(a: string, b: string, flag: bool) -> int64:
    sum = 0
    sum += a[:min(a.size(), b.size())] != b[$1]
    if sum != 0:
        return 1e18LL
    elif a.size() == b.size():
        return 0
    elif b.size() < a.size():
        return func(flag, a.substr(b.size()))
    else:
        return func(!flag, b.substr(a.size()))
def solve() -> int64:
    ret = 1e18LL
    for n in N:
        s = S[n]
        for i in s.size()+1:
            prefix = s.substr(0, i)
            prefix.reverse()
            if i < s.size():
                ret <?= func2(prefix, s.substr(i + 1), True) + C[n]
            ret <?= func2(prefix, s.substr(i), True) + C[n]
    return ret==1e18LL ? -1 : ret
print(solve())
