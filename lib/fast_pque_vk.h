template<typename T, int N, class Compare=std::less<T> > class fast_pque_vk {
private:
    static constexpr int L = std::tuple_size<T>::value-1;
    Compare compare;
    int count;
    bool lazy;
    fast_array<unsigned short, N> index;
    fast_array<T, N> data;
    inline void adjust(int now, const T & value) {
        assert(0<=count && count<=N);
        assert(0<=now && now<count);
        int nxt = now+now+1;
        while(nxt+1<count) {
            if(compare(data[nxt], data[nxt+1])) {
                ++nxt;
            }
            if(!compare(value, data[nxt])) {
                data[now] = value;
                index[std::get<L>(value)] = now;
                return;
            }
            data[now] = data[nxt];
            index[std::get<L>(data[now])] = now;
            now = nxt;
            nxt = now+now+1;
        }
        if(nxt<count) {
            if(compare(value, data[nxt])) {
                data[now] = data[nxt];
                index[std::get<L>(data[now])] = now;
                data[nxt] = value;
                index[std::get<L>(value)] = nxt;
                return;
            }
        }
        data[now] = value;
        index[std::get<L>(value)] = now;
    }
    inline void pop_() {
        assert(0<count);
        --count;
        if(1<=count) {
            adjust(0, data[count]);
        }
    }
    inline void swap_(int now, const T & value) {
        assert(0<=count && count<=N);
        assert(!lazy);
        assert(0<=now && now<count);
        if(1<=now) {
            int nxt = (now-1)>>1;
            if(!compare(value, data[nxt])) {
                while(compare(data[nxt], value)) {
                    data[now] = data[nxt];
                    index[std::get<L>(data[now])] = now;
                    now = nxt;
                    if(now==0) {
                        break;
                    }
                    nxt = (now-1)>>1;
                }
                data[now] = value;
                index[std::get<L>(value)] = now;
                return;
            }
        }
        adjust(now, value);
    }
public:
    inline fast_pque_vk() : count(0), lazy(false) {
        assert(N<65536);
    }
    inline void clear() {
        count = 0;
        lazy = false;
    }
    inline bool empty() const {
        assert(0<=count && count<=N);
        if(lazy) {
            return count<=1;
        }
        return count==0;
    }
    inline bool full() const {
        assert(0<=count && count<=N);
        if(lazy) {
            return false;
        }
        return count==N;
    }
    inline const T & top() {
        assert(0<=count && count<=N);
        if(lazy) {
            lazy = false;
            pop_();
        }
        assert(0<count);
        return data[0];
    }
    inline void push(const T & value) {
        const auto & key = std::get<L>(value);
        int i = index[key];
        if((lazy ? 1 : 0)<=i && i<count && std::get<L>(data[i])==key) {
            if(lazy) {
                lazy = false;
                pop_();
                i = index[key];
                assert(0<=i && i<count && std::get<L>(data[i])==key);
            }
            swap_(i, value);
        }
        else if(lazy) {
            lazy = false;
            adjust(0, value);
        }
        else {
            assert(0<=count && count<=N);
            assert(count<N);
            ++count;
            if(3<=count) {
                int now = count-1;
                int nxt = (now-1)>>1;
                while(compare(data[nxt], value)) {
                    data[now] = data[nxt];
                    index[std::get<L>(data[now])] = now;
                    now = nxt;
                    if(now==0) {
                        break;
                    }
                    nxt = (now-1)>>1;
                }
                data[now] = value;
                index[std::get<L>(value)] = now;
            }
            else if(count==2) {
                if(compare(data[0], value)) {
                    data[1] = data[0];
                    index[std::get<L>(data[1])] = 1;
                    data[0] = value;
                    index[std::get<L>(value)] = 0;
                }
                else {
                    data[1] = value;
                    index[std::get<L>(value)] = 1;
                }
            }
            else {
                data[0] = value;
                index[std::get<L>(value)] = 0;
            }
        }
    }
    inline void pop() {
        assert(0<=count && count<=N);
        if(lazy) {
            pop_();
        }
        else {
            lazy = true;
        }
        assert(0<count);
    }
    template<class... TyArgs> inline void emplace(TyArgs&&... args) {
        assert(0<=count && count<=N);
        T value;
        ::new(&value)T(std::forward<TyArgs>(args)...);
        push(value);
    }
    inline void erase(int key) {
        assert(0<=key && key<N);
        assert(0<=count && count<=N);
        if(lazy) {
            lazy = false;
            pop_();
        }
        int i = index[key];
        if(0<=i && i<count && std::get<L>(data[i])==key) {
            --count;
            if(i!=count) {
                swap_(i, data[count]);
            }
        }
    }
    inline const T & operator [] (int i) {
        assert(0<=count && count<=N);
        if(lazy) {
            lazy = false;
            pop_();
        }
        assert(0<=i && i<count);
        return data[i];
    }
    inline const T & get(int i) {
        assert(0<=count && count<=N);
        if(lazy) {
            lazy = false;
            pop_();
        }
        assert(0<=i && i<count);
        return data[i];
    }
    inline int size() const {
        assert(0<=count && count<=N);
        if(lazy) {
            return count - 1;
        }
        return count;
    }
};
