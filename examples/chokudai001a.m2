W = 30
WW = W * W
 
A: vector!(char)
A[:WW] = inputInt()
P: vector!(short)
C[:WW][:5][:0] = char(0)
R[:WW] = char(0)
S: vector!(char)
S[:WW] = 0
D = [0, -1, +1, -W, +W]
for i in range(WW):
    x, y = i%W, i/W
    c = 0
    cnt = 0
    if 0<x and A[i-1] < A[i]:
        C[i][:5].push_back(1)
        C[i][1].pop_back()
        S[i] = 1
        ++cnt
    if x<W-1 and A[i+1] < A[i]:
        C[i][:5].push_back(2)
        C[i][2].pop_back()
        S[i] = 2
        ++cnt
    if 0<y and A[i-W] < A[i]:
        C[i][:5].push_back(3)
        C[i][3].pop_back()
        S[i] = 3
        ++cnt
    if y<W-1 and A[i+W] < A[i]:
        C[i][:5].push_back(4)
        C[i][4].pop_back()
        S[i] = 4
        ++cnt
    if S[i]:
        ++R[i+D[S[i]]]
    if 2<=cnt:
        P.push_back(i)
limit = 9.8
T0 = 10
while True:
    t = getTime()
    if limit<=t:
        break
    progress = t / limit
    remain = 1 - progress
    T = T0 * remain
    if gvFps(t):
        gvArrow($1%W, $1/W, (n@$1+D[S[:WW]])%W, n/W, gvRGB(128, 128, 128))
        gvText($1%W, $1/W, 0.3, "%d", A[:WW])
        gvTextLeft(W, 0, "t: %f", t)
        gvTextLeft(W, 1, "T: %f", T)
    i = P[lrand49(P.size())]
    oldS = S[i]
    newS = C[i][oldS][lrand49(C[i][oldS].size())]
    oldJ = i+D[oldS]
    newJ = i+D[newS]
    dSc = 0
    if 1==R[oldJ]:
        dSc -= A[oldJ]
    if 0==R[newJ]:
        dSc += A[newJ]
    if drand49()<exp(dSc/T):
        S[i] = newS
        --R[oldJ]
        ++R[newJ]
for i0 in range(WW):
    if R[i0]!=0:
        continue
    while A[i0]:
        i = i0
        while True:
            if not A[i]:
                break
            --A[i]
            print(i/W+1, i%W+1)
            if not S[i]:
                break
            j = i + D[S[i]]
            if A[i]!=A[j]:
                break
            i = j
