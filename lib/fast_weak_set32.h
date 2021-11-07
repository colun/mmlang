template<int N=65536>class fast_weak_set32 {
private:
    const int mask = N - 1;
    fast_array<unsigned short, N> memo;
public:
    inline void clear() {
        memset(memo.data(), 0, sizeof(memo));
    }
    inline fast_weak_set32() {
        assert((N&(N-1))==0);
    }
    inline bool test(int hash) const {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 1;
        }
        return memo[key]==hi;
    }
    inline void set(int hash) {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 1;
        }
        memo[key] = hi;
    }
    inline void reset(int hash) {
        unsigned int key = hash & mask;
        memo[key] = 0;
    }
    inline bool testWithSet(int hash) {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 1;
        }
        if(memo[key]==hi) {
            return true;
        }
        memo[key] = hi;
        return false;
    }
};
