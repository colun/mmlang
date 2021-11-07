template<typename T, int N, class Compare=std::less<T> > class fast_pque {
private:
    Compare compare;
    int count;
    bool lazy;
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
                return;
            }
            data[now] = data[nxt];
            now = nxt;
            nxt = now+now+1;
        }
        if(nxt<count) {
            if(compare(value, data[nxt])) {
                data[now] = data[nxt];
                data[nxt] = value;
                return;
            }
        }
        data[now] = value;
    }
    inline void pop_() {
        assert(0<count);
        --count;
        if(1<=count) {
            adjust(0, data[count]);
        }
    }
public:
    inline fast_pque() : count(0), lazy(false) {
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
    inline void swap_top(const T & value) {
        adjust(0, value);
    }
    inline void push(const T & value) {
        if(lazy) {
            lazy = false;
            swap_top(value);
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
                    now = nxt;
                    if(now==0) {
                        break;
                    }
                    nxt = (now-1)>>1;
                }
                data[now] = value;
            }
            else if(count==2) {
                if(compare(data[0], value)) {
                    data[1] = data[0];
                    data[0] = value;
                }
                else {
                    data[1] = value;
                }
            }
            else {
                data[0] = value;
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
    inline void swap(int now, const T & value) {
        assert(0<=count && count<=N);
        if(lazy) {
            lazy = false;
            pop_();
        }
        assert(0<=now && now<count);
        if(1<=now) {
            int nxt = (now-1)>>1;
            if(!compare(value, data[nxt])) {
                while(compare(data[nxt], value)) {
                    data[now] = data[nxt];
                    now = nxt;
                    if(now==0) {
                        break;
                    }
                    nxt = (now-1)>>1;
                }
                data[now] = value;
                return;
            }
        }
        adjust(now, value);
    }
    inline void remove(int i) {
        assert(0<=count && count<=N);
        if(lazy) {
            lazy = false;
            pop_();
        }
        assert(0<=i && i<count);
        --count;
        if(i!=count) {
            swap(i, data[count]);
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
