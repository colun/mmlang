template<typename T, int N, class Compare=std::less<T> > class xpque {
private:
    Compare compare;
    int count;
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
                xmem::modify(&data[now], sizeof(T));
                data[now] = value;
                return;
            }
            xmem::modify(&data[now], sizeof(T));
            data[now] = data[nxt];
            now = nxt;
            nxt = now+now+1;
        }
        if(nxt<count) {
            if(compare(value, data[nxt])) {
                xmem::modify(&data[now], sizeof(T));
                data[now] = data[nxt];
                xmem::modify(&data[nxt], sizeof(T));
                data[nxt] = value;
                return;
            }
        }
        xmem::modify(&data[now], sizeof(T));
        data[now] = value;
    }
public:
    inline xpque() : count(0) {
        xmem::init(this);
    }
    inline void clear() {
        xmem::modify(&count, sizeof(count));
        count = 0;
    }
    inline bool empty() const {
        assert(0<=count && count<=N);
        return count==0;
    }
    inline bool full() const {
        assert(0<=count && count<=N);
        return count==N;
    }
    inline const T & top() const {
        assert(0<=count && count<=N);
        assert(0<count);
        return data[0];
    }
    inline void push(const T & value) {
        assert(0<=count && count<=N);
        assert(count<N);
        xmem::modify(&count, sizeof(count));
        ++count;
        if(3<=count) {
            int now = count-1;
            int nxt = (now-1)>>1;
            while(compare(data[nxt], value)) {
                xmem::modify(&data[now], sizeof(T));
                data[now] = data[nxt];
                now = nxt;
                if(now==0) {
                    break;
                }
                nxt = (now-1)>>1;
            }
            xmem::modify(&data[now], sizeof(T));
            data[now] = value;
        }
        else if(count==2) {
            if(compare(data[0], value)) {
                xmem::modify(&data[0], sizeof(T)*2);
                data[1] = data[0];
                data[0] = value;
            }
            else {
                xmem::modify(&data[1], sizeof(T));
                data[1] = value;
            }
        }
        else {
            xmem::modify(&data[0], sizeof(T));
            data[0] = value;
        }
    }
    inline void swap_top(const T & value) {
        adjust(0, value);
    }
    inline void pop() {
        assert(0<=count && count<=N);
        assert(0<count);
        xmem::modify(&count, sizeof(count));
        --count;
        if(1<=count) {
            adjust(0, data[count]);
        }
    }
    template<class... TyArgs> inline void emplace(TyArgs&&... args) {
        assert(0<=count && count<=N);
        T value;
        ::new(&value)T(std::forward<TyArgs>(args)...);
        push(value);
    }
    inline void swap(int now, const T & value) {
        assert(0<=count && count<=N);
        assert(0<=now && now<count);
        if(1<=now) {
            int nxt = (now-1)>>1;
            if(!compare(value, data[nxt])) {
                while(compare(data[nxt], value)) {
                    xmem::modify(&data[now], sizeof(T));
                    data[now] = data[nxt];
                    now = nxt;
                    if(now==0) {
                        break;
                    }
                    nxt = (now-1)>>1;
                }
                xmem::modify(&data[now], sizeof(T));
                data[now] = value;
                return;
            }
        }
        adjust(now, value);
    }
    inline void remove(int i) {
        assert(0<=count && count<=N);
        assert(0<=i && i<count);
        xmem::modify(&count, sizeof(count));
        --count;
        if(i!=count) {
            swap(i, data[count]);
        }
    }
    inline const T & operator [] (int i) const {
        assert(0<=count && count<=N);
        assert(0<=i && i<count);
        return data[i];
    }
    inline const T & get(int i) const {
        assert(0<=count && count<=N);
        assert(0<=i && i<count);
        return data[i];
    }
    inline int size() const {
        assert(0<=count && count<=N);
        return count;
    }
};
