template<class T, int N=1048576>class fast_weak_map64 {
private:
    const unsigned int mask = N - 1;
    fast_array<tuple<unsigned int, T>, N> memo;
public:
    inline void clear() {
        memset(memo.data(), 0, sizeof(memo));
    }
    inline fast_weak_map64() {
        assert((N&(N-1))==0);
    }
    inline bool test(long long hash) const {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
        if(hi==0) {
            hi = 1;
        }
        return get<0>(memo[key])==hi;
    }
    inline bool testget(long long hash, T & value) const {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
        if(hi==0) {
            hi = 1;
        }
        if(get<0>(memo[key])==hi) {
            value = get<1>(memo[key]);
            return true;
        }
        return false;
    }
    inline void set(long long hash, const T & value) {
        unsigned int key = (unsigned int)hash & mask;
        unsigned int hi = ((unsigned long long)hash >> 32);
        if(hi==0) {
            hi = 1;
        }
        memo[key] = tuple<unsigned int, T>(hi, value);
    }
    inline const T & get(long long hash) const {
        return get<1>(memo[(unsigned int)hash & mask]);
    }
    inline void reset(long long hash) {
        unsigned int key = (unsigned int)hash & mask;
        get<0>(memo[key]) = 0;
    }
};
