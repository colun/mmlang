template<typename T, int N> class xdpque {
private:
    int count;
    fast_array<T, N> data;
    inline void adjustDown(int now, const T & value) {
        while(true) {
            int nxt1 = now + now + 2;
            int nxt2 = nxt1 + 1;
            if(nxt2<count) {
                if(data[nxt1] < data[nxt2]) {
                    if(value < data[nxt2]) {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt2];
                        now = nxt2;
                    }
                    else {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = value;
                        return;
                    }
                }
                else {
                    if(value < data[nxt1]) {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt1];
                        now = nxt1;
                    }
                    else {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = value;
                        return;
                    }
                }
            }
            else if(nxt1<count) {
                if(value < data[nxt1]) {
                    xmem::modify(&data[now], sizeof(T));
                    data[now] = data[nxt1];
                    now = nxt1;
                }
                else {
                    xmem::modify(&data[now], sizeof(T));
                    data[now] = value;
                    return;
                }
            }
            else {
                break;
            }
        }
        int clz = __builtin_clz(now+2);
        int nxt = now+(1<<(30-clz));
        if(count<=nxt) {
            nxt = (nxt-2)>>1;
        }
        if(0<=nxt) {
            while(value < data[nxt]) {
                xmem::modify(&data[now], sizeof(T));
                data[now] = data[nxt];
                now = nxt;
                if(now<2) {
                    break;
                }
                nxt = (now-2)>>1;
            }
        }
        xmem::modify(&data[now], sizeof(T));
        data[now] = value;
    }
    inline void adjustUp(int now, const T & value) {
        while(true) {
            int nxt1 = now + now + 2;
            int nxt2 = nxt1 + 1;
            if(nxt2<count) {
                if(data[nxt2] < data[nxt1]) {
                    if(data[nxt2] < value) {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt2];
                        now = nxt2;
                    }
                    else {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = value;
                        return;
                    }
                }
                else {
                    if(data[nxt1] < value) {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt1];
                        now = nxt1;
                    }
                    else {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = value;
                        return;
                    }
                }
            }
            else if(nxt1<count) {
                if(data[nxt1] < value) {
                    xmem::modify(&data[now], sizeof(T));
                    data[now] = data[nxt1];
                    now = nxt1;
                }
                else {
                    xmem::modify(&data[now], sizeof(T));
                    data[now] = value;
                    return;
                }
            }
            else {
                break;
            }
        }
        int clz = __builtin_clz(now+2);
        int nxt = now+now-(1<<(31-clz))+2;
        if(count<=nxt) {
            nxt = (nxt-2)>>1;
        }
        if(0<=nxt) {
            while(data[nxt] < value) {
                xmem::modify(&data[now], sizeof(T));
                data[now] = data[nxt];
                now = nxt;
                if(now<2) {
                    break;
                }
                nxt = (now-2)>>1;
            }
        }
        xmem::modify(&data[now], sizeof(T));
        data[now] = value;
    }
public:
    inline xdpque() : count(0) {
        xmem::init(this, sizeof(*this));
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
    inline const T & large() const {
        assert(0<=count && count<=N);
        assert(0<count);
        return data[0];
    }
    inline const T & small() const {
        assert(0<=count && count<=N);
        assert(0<count);
        return count==1 ? data[0] : data[1];
    }
    inline void push(const T & value) {
        assert(0<=count && count<=N);
        assert(count<N);
        xmem::modify(&count, sizeof(count));
        ++count;
        if(3<=count) {
            int now = count-1;
            int clz = __builtin_clz(now+2);
            int nxt = (now-2)>>1;
            if(now<((3<<(30-clz))-2)) {//now<border
                if(data[nxt] < value) {
                    do {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt];
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    } while(data[nxt] < value);
                }
                else {
                    int nxt = (now+(1<<(30-clz))-2)>>1;
                    while(value < data[nxt]) {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt];
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    }
                }
            }
            else {
                if(value < data[nxt]) {
                    do {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt];
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    } while(value < data[nxt]);
                }
                else {
                    int nxt = now-(1<<(30-clz));
                    while(data[nxt] < value) {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt];
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    }
                }
            }
            xmem::modify(&data[now], sizeof(T));
            data[now] = value;
        }
        else if(count==2) {
            if(data[0] < value) {
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
    inline void pop_large() {
        assert(0<=count && count<=N);
        assert(0<count);
        xmem::modify(&count, sizeof(count));
        --count;
        if(1<=count) {
            adjustDown(0, data[count]);
        }
    }
    inline void pop_small() {
        assert(0<=count && count<=N);
        assert(0<count);
        xmem::modify(&count, sizeof(count));
        --count;
        if(2<=count) {
            adjustUp(1, data[count]);
        }
    }
    inline void swap_large(const T & value) {
        assert(0<=count && count<=N);
        assert(0<count);
        adjustDown(0, value);
    }
    inline void swap_small(const T & value) {
        assert(0<=count && count<=N);
        assert(0<count);
        if(2<=count) {
            adjustUp(1, value);
        }
        else {
            xmem::modify(&data[0], sizeof(T));
            data[0] = value;
        }
    }
    inline void push_large(const T & value) {
        assert(this);
        assert(0<=count && count<=N);
        if(!full()) {
            push(value);
        }
        else if(small()<value) {
            swap_small(value);
        }
    }
    inline void push_small(const T & value) {
        assert(0<=count && count<=N);
        if(!full()) {
            push(value);
        }
        else if(value<large()) {
            swap_large(value);
        }
    }
    template<class... TyArgs> inline void emplace(TyArgs&&... args) {
        assert(0<=count && count<=N);
        T value;
        ::new(&value)T(std::forward<TyArgs>(args)...);
        push(value);
    }
    template<class... TyArgs> inline void emplace_large(TyArgs&&... args) {
        assert(0<=count && count<=N);
        T value;
        ::new(&value)T(std::forward<TyArgs>(args)...);
        push_large(value);
    }
    template<class... TyArgs> inline void emplace_small(TyArgs&&... args) {
        assert(0<=count && count<=N);
        T value;
        ::new(&value)T(std::forward<TyArgs>(args)...);
        push_small(value);
    }
    inline void swap(int now, const T & value) {
        assert(0<=count && count<=N);
        assert(0<=now && now<count);
        int clz0 = __builtin_clz(now+2);
        if(now<((3<<(30-clz0))-2)) {
            if(2<=now) {
                int nxt = (now-2)>>1;
                if(data[nxt] < value) {
                    do {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt];
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    } while(data[nxt] < value);
                    xmem::modify(&data[now], sizeof(T));
                    data[now] = value;
                    return;
                }
            }
            adjustDown(now, value);
        }
        else {
            if(2<=now) {
                int nxt = (now-2)>>1;
                if(value < data[nxt]) {
                    do {
                        xmem::modify(&data[now], sizeof(T));
                        data[now] = data[nxt];
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    } while(value < data[nxt]);
                    xmem::modify(&data[now], sizeof(T));
                    data[now] = value;
                    return;
                }
            }
            adjustUp(now, value);
        }
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
