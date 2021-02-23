id, N, K = inputInt(), inputInt(), inputInt()
S[:N] = inputWord()
field[:10000] = S[$1/100][$1%100] - '1'
center = 4949

done: xarray!(char, 10000)
Q: array!(xvector!(int, 10000), 9)
answer: xvector!(int, 10000)
hash_seed: zobrist!(int, 10000)

done[:10000] = 0
best = vector!(int)()
best3 = 0

@beam(maxdepth=4, timelimit=2.9, maxwidth=1, verbose=false)
def search3(score, score0, color):
    s: stack!(int)
    for xy in Q[color]:
        s.push(xy)
    Q[color].clear()
    for xy in s:
        if done[xy]==2:
            continue
        c = field[xy]
        if c==color:
            x, y = xy % 100, xy / 100
            done[xy] = 2
            hash0 ^= hash_seed[xy]
            ++score0
            if x!=0:
                s.push(xy - 1)
            if x!=99:
                s.push(xy + 1)
            if y!=0:
                s.push(xy - 100)
            if y!=99:
                s.push(xy + 100)
        elif done[xy]==0:
            done[xy] = 1
            Q[c].push_back(xy)
    for c in 9:
        if !Q[c].empty():
            if best3<score0 + Q[c].size():
                best3 = score0 + Q[c].size()
            search3(score0 + Q[c].size(), score0, c)

@beam(maxdepth=170, timelimit=2.9, maxwidth=800)
def search2(hash, hash0, score, score0, color):
    answer.push_back(color)
    s: stack!(int)
    for xy in Q[color]:
        s.push(xy)
    Q[color].clear()
    for xy in s:
        if done[xy]==2:
            continue
        c = field[xy]
        if c==color:
            x, y = xy % 100, xy / 100
            done[xy] = 2
            hash0 ^= hash_seed[xy]
            ++score0
            if x!=0:
                s.push(xy - 1)
            if x!=99:
                s.push(xy + 1)
            if y!=0:
                s.push(xy - 100)
            if y!=99:
                s.push(xy + 100)
        elif done[xy]==0:
            done[xy] = 1
            Q[c].push_back(xy)
    if score0==10000:
        if best.empty() or answer.size() < best.size():
            best.assign(answer.begin(), answer.end())
    for c in 9:
        if !Q[c].empty():
            best3 = 0
            search3(score0 + Q[c].size(), score0, c)
            search2(hash0+c, hash0, best3, score0, c)
Q[field[center]].push_back(center)
search2(0, 0, 0, 0, field[center])
print(best.size()-1)
print(center/100+1, center%100+1, best[1:]+1)


