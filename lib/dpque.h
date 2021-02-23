template<typename T> struct dpque_dummy_callback {
    inline dpque_dummy_callback() {
    }
    inline void operator () (const T & o, int pos) {
    }
};

template<typename T, typename F=dpque_dummy_callback<T> > class dpque {
private:
    F callback;
    std::vector<T> data;
    inline void adjustDown(int now, const T & value) {
        int count = size();
        while(true) {
            int nxt1 = now + now + 2;
            int nxt2 = nxt1 + 1;
            if(nxt2<count) {
                if(data[nxt1] < data[nxt2]) {
                    if(value < data[nxt2]) {
                        data[now] = std::move(data[nxt2]);
                        callback(data[now], now);
                        now = nxt2;
                    }
                    else {
                        data[now] = std::move(value);
                        callback(data[now], now);
                        return;
                    }
                }
                else {
                    if(value < data[nxt1]) {
                        data[now] = std::move(data[nxt1]);
                        callback(data[now], now);
                        now = nxt1;
                    }
                    else {
                        data[now] = std::move(value);
                        callback(data[now], now);
                        return;
                    }
                }
            }
            else if(nxt1<count) {
                if(value < data[nxt1]) {
                    data[now] = std::move(data[nxt1]);
                    callback(data[now], now);
                    now = nxt1;
                }
                else {
                    data[now] = std::move(value);
                    callback(data[now], now);
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
                data[now] = std::move(data[nxt]);
                callback(data[now], now);
                now = nxt;
                if(now<2) {
                    break;
                }
                nxt = (now-2)>>1;
            }
        }
        data[now] = std::move(value);
        callback(data[now], now);
    }
    inline void adjustUp(int now, const T & value) {
        int count = size();
        while(true) {
            int nxt1 = now + now + 2;
            int nxt2 = nxt1 + 1;
            if(nxt2<count) {
                if(data[nxt2] < data[nxt1]) {
                    if(data[nxt2] < value) {
                        data[now] = std::move(data[nxt2]);
                        callback(data[now], now);
                        now = nxt2;
                    }
                    else {
                        data[now] = std::move(value);
                        callback(data[now], now);
                        return;
                    }
                }
                else {
                    if(data[nxt1] < value) {
                        data[now] = std::move(data[nxt1]);
                        callback(data[now], now);
                        now = nxt1;
                    }
                    else {
                        data[now] = std::move(value);
                        callback(data[now], now);
                        return;
                    }
                }
            }
            else if(nxt1<count) {
                if(data[nxt1] < value) {
                    data[now] = std::move(data[nxt1]);
                    callback(data[now], now);
                    now = nxt1;
                }
                else {
                    data[now] = std::move(value);
                    callback(data[now], now);
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
                data[now] = std::move(data[nxt]);
                callback(data[now], now);
                now = nxt;
                if(now<2) {
                    break;
                }
                nxt = (now-2)>>1;
            }
        }
        data[now] = std::move(value);
        callback(data[now], now);
    }
public:
    inline dpque() {
    }
    inline void clear() {
        for(T & o : data) {
            callback(o, -1);
        }
        data.clear();
    }
    inline ~dpque() {
        clear();
    }
    inline void clearWithoutCallback() {
        data.clear();
    }
    inline bool empty() const {
        return data.empty();
    }
    inline void push(const T & value) {
        int now = size();
        if(2<=now) {
            int clz = __builtin_clz(now+2);
            int nxt = (now-2)>>1;
            data.emplace_back();
            if(now<((3<<(30-clz))-2)) {
                if(data[nxt] < value) {
                    do {
                        data[now] = std::move(data[nxt]);
                        callback(data[now], now);
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
                        data[now] = std::move(data[nxt]);
                        callback(data[now], now);
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
                        data[now] = std::move(data[nxt]);
                        callback(data[now], now);
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
                        data[now] = std::move(data[nxt]);
                        callback(data[now], now);
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    }
                }
            }
            data[now] = std::move(value);
            callback(data[now], now);
        }
        else if(now==1) {
            if(data[0] < value) {
                data.emplace_back(data[0]);
                callback(data[1], 1);
                data[0] = std::move(value);
                callback(data[0], 0);
            }
            else {
                data.emplace_back(std::move(value));
                callback(data[1], 1);
            }
        }
        else {
            assert(empty());
            data.push_back(std::move(value));
            callback(data[0], 0);
        }
    }
    inline const T & large() const {
        assert(!empty());
        return data[0];
    }
    inline const T & small() const {
        assert(!empty());
        return size()==1 ? data[0] : data[1];
    }
    inline void pop_large() {
        assert(!empty());
        if(size()==1) {
            callback(data[0], -1);
            data.pop_back();
        }
        else {
            T o = std::move(data.back());
            data.pop_back();
            callback(data[0], -1);
            adjustDown(0, o);
        }
    }
    inline void pop_small() {
        assert(!empty());
        if(size()==1) {
            callback(data[0], -1);
            data.pop_back();
        }
        else {
            callback(data[1], -1);
            if(size()==2) {
                data.pop_back();
            }
            else {
                T o = std::move(data.back());
                data.pop_back();
                adjustUp(1, o);
            }
        }
    }
    inline void swap_large(const T & value) {
        assert(!empty());
        callback(data[0], -1);
        adjustDown(0, value);
    }
    inline void swap_small(const T & value) {
        assert(!empty());
        if(2<=size()) {
            callback(data[1], -1);
            adjustUp(1, value);
        }
        else {
            callback(data[0], -1);
            data[0] = std::move(value);
            callback(data[0], 0);
        }
    }
    inline void push_large(const T & value, int size_) {
        if(size()<size_) {
            push(value);
        }
        else if(small()<value) {
            swap_small(value);
        }
    }
    inline void push_small(const T & value, int size_) {
        if(size()<size_) {
            push(value);
        }
        else if(value<large()) {
            swap_large(value);
        }
    }
    template<class... TyArgs> inline void emplace(TyArgs&&... args) {
        T value(std::forward<TyArgs>(args)...);
        push(value);
    }
    template<class... TyArgs> inline void emplace_large(TyArgs&&... args) {
        T value(std::forward<TyArgs>(args)...);
        push_large(value);
    }
    template<class... TyArgs> inline void emplace_small(TyArgs&&... args) {
        T value(std::forward<TyArgs>(args)...);
        push_small(value);
    }
    inline void swap(int now, const T & value) {
        assert(0<=now && now<size());
        callback(data[now], -1);
        int clz0 = __builtin_clz(now+2);
        if(now<((3<<(30-clz0))-2)) {
            if(2<=now) {
                int nxt = (now-2)>>1;
                if(data[nxt] < value) {
                    do {
                        data[now] = std::move(data[nxt]);
                        callback(data[now], now);
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    } while(data[nxt] < value);
                    data[now] = std::move(value);
                    callback(data[now], now);
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
                        data[now] = std::move(data[nxt]);
                        callback(data[now], now);
                        now = nxt;
                        if(now<2) {
                            break;
                        }
                        nxt = (now-2)>>1;
                    } while(value < data[nxt]);
                    data[now] = std::move(value);
                    callback(data[now], now);
                    return;
                }
            }
            adjustUp(now, value);
        }
    }
    inline void remove(int i) {
        assert(0<=i && i<size());
        if(i+1==size()) {
            callback(data[i], -1);
            data.pop_back();
        }
        else {
            T o = std::move(data.back());
            data.pop_back();
            swap(i, o);
        }
    }
    inline const T & operator [] (int i) const {
        assert(0<=i && i<size());
        return data[i];
    }
    inline const T & get(int i) const {
        assert(0<=i && i<size());
        return data[i];
    }
    inline int size() const {
        return data.size();
    }
    typedef typename std::vector<T>::const_iterator const_iterator;
    inline const_iterator begin() const {
        return data.begin();
    }
    inline const_iterator end() const {
        return data.end();
    }
};
