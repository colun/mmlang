N = inputInt()
L[:N] = inputInt()
cnt = 0
if L[k@:N] < L[j@:k] + L[i@:j] and L[j] < L[i] + L[k] and L[i] < L[j] + L[k] and L[i]!=L[j]!=L[k]!=L[i]:
    ++cnt
print(cnt)
