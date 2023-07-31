template<class T> class xvar {
private:
    T data;
public:
    inline xvar(const T & o) : data(o) {
        xmem::init(this, sizeof(*this));
    }
    inline xvar() : data() {
        xmem::init(this, sizeof(*this));
    }
    inline void operator = (const T & o) {
        if(data!=o) {
            xmem::modify(&data, sizeof(data));
            data = o;
        }
    }
    inline void operator += (const T & o) {
        T v = data + o;
        if(data!=v) {
            xmem::modify(&data, sizeof(data));
            data = v;
        }
    }
    inline void operator -= (const T & o) {
        T v = data - o;
        if(data!=v) {
            xmem::modify(&data, sizeof(data));
            data = v;
        }
    }
    inline void operator *= (const T & o) {
        T v = data * o;
        if(data!=v) {
            xmem::modify(&data, sizeof(data));
            data = v;
        }
    }
    inline void operator /= (const T & o) {
        T v = data / o;
        if(data!=v) {
            xmem::modify(&data, sizeof(data));
            data = v;
        }
    }
    inline void operator &= (const T & o) {
        T v = data & o;
        if(data!=v) {
            xmem::modify(&data, sizeof(data));
            data = v;
        }
    }
    inline void operator |= (const T & o) {
        T v = data | o;
        if(data!=v) {
            xmem::modify(&data, sizeof(data));
            data = v;
        }
    }
    inline void operator ^= (const T & o) {
        T v = data ^ o;
        if(data!=v) {
            xmem::modify(&data, sizeof(data));
            data = v;
        }
    }
    inline xvar<T> & operator ++ () {
        xmem::modify(&data, sizeof(data));
        ++data;
        return *this;
    }
    inline T operator ++ (int) {
        xmem::modify(&data, sizeof(data));
        T v = data;
        ++data;
        return v;
    }
    inline xvar<T> & operator -- () {
        xmem::modify(&data, sizeof(data));
        --data;
        return *this;
    }
    inline T operator -- (int) {
        xmem::modify(&data, sizeof(data));
        T v = data;
        --data;
        return v;
    }
    inline T value() const {
        return data;
    }
    inline operator T() {
        return data;
    }
};
