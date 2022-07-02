template<int B=20>class fast_weak_counter64 {
private:
    static const unsigned int N = 1 << B;
    static const unsigned int mask = N - 1;
    fast_array<tuple<unsigned int, unsigned int>, N> memo;
public:
    inline void clear() {
        memset(memo.data(), 0, sizeof(memo));
    }
    inline fast_weak_counter64() {
        assert(1<=B);
        assert(B<=30);
    }
    inline int count(long long hash) {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
        if(hi==0) {
            hi = 0x80000000;
        }
        if(std::get<0>(memo[key])!=hi) {
            memo[key] = tuple<unsigned int, unsigned int>(hi, 0);
        }
        return ++std::get<1>(memo[key]);
    }
    inline int get(long long hash) const {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
        if(hi==0) {
            hi = 0x80000000;
        }
        if(std::get<0>(memo[key])!=hi) {
            return 0;
        }
        return std::get<1>(memo[key]);
    }
    inline void reset(long long hash) {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
        if(hi==0) {
            hi = 0x80000000;
        }
        if(std::get<0>(memo[key])==hi) {
            std::get<1>(memo[key]) = 0;
        }
    }
};
