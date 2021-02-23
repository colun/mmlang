template<int N=16777216>class fast_weak_set64 {
private:
    const unsigned int mask = N - 1;
    fast_array<unsigned int, N> memo;
public:
    inline void clear() {
        memset(memo.data(), 0, sizeof(memo));
    }
    inline fast_weak_set64() {
        assert((N&(N-1))==0);
    }
    inline bool test(long long hash) const {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
        if(hi==0) {
            hi = 1;
        }
        return memo[key]==hi;
    }
    inline void set(long long hash) {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
        if(hi==0) {
            hi = 1;
        }
        memo[key] = hi;
    }
    inline void reset(long long hash) {
        unsigned int key = (unsigned int)hash & mask;
        memo[key] = 0;
    }
    inline bool testWithSet(long long hash) {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
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
