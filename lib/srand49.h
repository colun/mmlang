inline void srand49(int seed) {
    mrand49$state = seed + 1442695040888963407ULL;
}
inline void srand49() {
    int clk;
    __asm__ __volatile__ ("rdtsc" : "=a"(*(unsigned int*)&clk) : : "%rdx");
    srand49(clk);
}
