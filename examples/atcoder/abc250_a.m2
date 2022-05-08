# atcoder: abc250_a ( https://atcoder.jp/contests/abc250/tasks/abc250_a )
H, W = inputInt(), inputInt()
R, C = inputInt(), inputInt()
ret = 0
if 2<=R:
    ++ret
if 2<=C:
    ++ret
if R<H:
    ++ret
if C<W:
    ++ret
print(ret)
