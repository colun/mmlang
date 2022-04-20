# atcoder: abc175_e
R, C, K = inputInt(), inputInt(), inputInt()
rcv[:K] = inputInt() - 1, inputInt() - 1, inputInt()
rcv.sort()
V[:R][:C] = 0
for r, c, v in rcv:
    V[r][c] = v
@memo(size=(R, C, 4))
def solve(r, c, i)->int64:
    ret = (1 <= i ? V[r][c]: 0LL)
    if 1 <= r:
        if i == 0:
            ret >?= solve(r - 1, c, : 4)
        else:
            ret >?= solve(r - 1, c, : 4) + V[r][c]
    if 1 <= c:
        ret >?= solve(r, c - 1, : i) + V[r][c]
        ret >?= solve(r, c - 1, i)
    return ret
print(solve(R - 1, C - 1, 3))
