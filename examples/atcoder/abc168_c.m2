# atcoder: abc168_c
A, B, H, M = inputInt(), inputInt(), inputInt(), inputInt()
M /= 60.0
H = (H + M) / 12.0
dx = cos(M * 2 * M_PI) * B - cos(H * 2 * M_PI) * A
dy = sin(M * 2 * M_PI) * B - sin(H * 2 * M_PI) * A
printf("%.20f\n", sqrt(dx*dx+dy*dy))
