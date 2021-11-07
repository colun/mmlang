# atcoder: https://sumitrust2019.contest.atcoder.jp/tasks/sumitb2019_d
N, S = inputInt(), inputWord()
one[:10] = 0
two[:100] = 0
three[:1000] = 0
for s in S:
    c = int(s-'0')
    if two[:]:
        three[$1*10+c] = 1
    if one[:]:
        two[$1*10+c] = 1
    one[c] = 1
print(sum(three[:]))
