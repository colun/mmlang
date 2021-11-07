# atcoder: abc147_a
N = 3
A[i@:N] = inputInt()
assert 1 <= A[:] <= 13
sum = 0
sum += A[:]
print(22 <= sum ? "bust": "win")
