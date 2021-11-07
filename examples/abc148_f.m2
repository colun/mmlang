# atcoder: abc148_f
N = inputInt()
T[:N] = 0
U = queue!(int)()
V = queue!(int)()
U.emplace(inputInt() - 1)
V.emplace(inputInt() - 1)
E[:N] = vector!(int)()
if True or (: N - 1):
    a, b = inputInt() - 1, inputInt() - 1
    E[a].emplace_back(b)
    E[b].emplace_back(a)

turn = 0
cnt = 0
while True:
    assert not (U.empty() and V.empty())
    for u in move(U):
        T[u] |= 1
        if !(T[u] & 2):
            ++cnt
        if !(T[e@E[u][:]] & 1):
            U.emplace(e)
    for v in move(V):
        T[v] |= 2
        if T[v] & 1:
            --cnt
        if !(T[e@E[v][:]] & 2):
            V.emplace(e)
    if cnt == 0:
        break
    ++turn
print(turn - 1)
