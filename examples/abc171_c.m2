N = inputInt64()
ret = string()
while N:
    --N
    ret.push_back('a' + (N % 26))
    N /= 26
ret.reverse()
print(ret)
