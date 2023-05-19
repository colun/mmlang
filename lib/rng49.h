class rng49 {
public:
    unsigned long long state;
    rng49() {
        int clk;
        __asm__ __volatile__ ("rdtsc" : "=a"(*(unsigned int*)&clk) : : "%rdx");
        state = clk + 1442695040888963407ULL;
    }
    inline void srand49(int seed) {
        state = seed + 1442695040888963407ULL;
    }
    inline void srand49() {
        int clk;
        __asm__ __volatile__ ("rdtsc" : "=a"(*(unsigned int*)&clk) : : "%rdx");
        this->srand49(clk);
    }
    inline int mrand49() {
        state *= 6364136223846793005ULL;
        state += 1442695040888963407ULL;
        unsigned int ret = ((state>>18)^state) >> 27;
        unsigned int rot = (state>>59);
        return (ret>>rot) | (ret<<-rot);
    }
    inline int mrand49(int r) {
        assert(r);
        return asm_mul_hi(this->mrand49(), r);
    }
    inline long long mmrand49() {
        int v = this->mrand49();
        return ((long long)v << 32) | this->mrand49();
    }
    inline int lrand49() {
        return this->mrand49() & 0x7FFFFFFF;
    }
    inline int lrand49(int r) {
        assert(1<=r);
        return asm_mul_hi(this->mrand49(), r);
    }
    inline double drand49() {
        return ((unsigned int)this->mrand49() + 0.5) * (1.0/4294967296.0);
    }
};
