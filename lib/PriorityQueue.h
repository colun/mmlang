template<typename T> class PriorityQueue {
private:
    vector<T> data;
    inline void adjust(int now, int count, const T & value) {
        assert(0<=count && count<=data.size());
        assert(0<=now && now<count);
        int nxt = now+now+1;
        while(nxt+1<count) {
            if(data[nxt+1] < data[nxt]) {
                ++nxt;
            }
            if(value < data[nxt]) {
                data[now] = value;
                return;
            }
            data[now] = data[nxt];
            now = nxt;
            nxt = now+now+1;
        }
        if(nxt<count) {
            if(data[nxt] < value) {
                data[now] = data[nxt];
                data[nxt] = value;
                return;
            }
        }
        data[now] = value;
    }
public:
    inline PriorityQueue() : data() {}
    inline void clear() { data.clear(); }
    inline bool empty() const { return data.empty(); }
    inline const T & top() const {
        assert(0<data.size());
        return data[0];
    }
    inline void push(const T & value) {
        data.push_back(value);
        if(3<=data.size()) {
            int now = data.size()-1;
            int nxt = (now-1)>>1;
            while(value < data[nxt]) {
                data[now] = data[nxt];
                now = nxt;
                if(now==0) {
                    break;
                }
                nxt = (now-1)>>1;
            }
            data[now] = value;
        }
        else if(data.size()==2 && value < data[0]) {
            data[1] = data[0];
            data[0] = value;
        }
    }
    inline void swap_top(const T & value) {
        adjust(0, data.size(), value);
    }
    inline void pop() {
        assert(0<data.size());
        if(2<=data.size()) {
            adjust(0, data.size()-1, data[data.size()-1]);
        }
        data.pop_back();
    }
    template<class... TyArgs> inline void emplace(TyArgs&&... args) {
        T value(std::forward<TyArgs>(args)...);
        push(value);
    }
    inline int size() const { return data.size(); }
};
