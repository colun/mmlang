template<class T> class xref {
private:
    T * pointer;
public:
    inline xref(T * p) : pointer(p) {
    }
    inline void operator = (const T & o) {
        if(*pointer!=o) {
            xmem::modify(pointer, sizeof(T));
            *pointer = o;
        }
    }
    inline void operator += (const T & o) {
        T v = *pointer + o;
        if(*pointer!=v) {
            xmem::modify(pointer, sizeof(T));
            *pointer = v;
        }
    }
    inline void operator -= (const T & o) {
        T v = *pointer - o;
        if(*pointer!=v) {
            xmem::modify(pointer, sizeof(T));
            *pointer = v;
        }
    }
    inline void operator *= (const T & o) {
        T v = *pointer * o;
        if(*pointer!=v) {
            xmem::modify(pointer, sizeof(T));
            *pointer = v;
        }
    }
    inline void operator /= (const T & o) {
        T v = *pointer / o;
        if(*pointer!=v) {
            xmem::modify(pointer, sizeof(T));
            *pointer = v;
        }
    }
    inline void operator &= (const T & o) {
        T v = *pointer & o;
        if(*pointer!=v) {
            xmem::modify(pointer, sizeof(T));
            *pointer = v;
        }
    }
    inline void operator |= (const T & o) {
        T v = *pointer | o;
        if(*pointer!=v) {
            xmem::modify(pointer, sizeof(T));
            *pointer = v;
        }
    }
    inline void operator ^= (const T & o) {
        T v = *pointer ^ o;
        if(*pointer!=v) {
            xmem::modify(pointer, sizeof(T));
            *pointer = v;
        }
    }
    inline xref<T> & operator ++ () {
        xmem::modify(pointer, sizeof(T));
        ++(*pointer);
        return *this;
    }
    inline T operator ++ (int) {
        xmem::modify(pointer, sizeof(T));
        T v = *pointer;
        ++(*pointer);
        return v;
    }
    inline xref<T> & operator -- () {
        xmem::modify(pointer, sizeof(T));
        --(*pointer);
        return *this;
    }
    inline T operator -- (int) {
        xmem::modify(pointer, sizeof(T));
        T v = *pointer;
        --(*pointer);
        return v;
    }
    inline T value() const {
        return *pointer;
    }
    inline operator T() const {
        return *pointer;
    }
};
