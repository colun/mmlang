struct _range_ite {
    int i;
    int s;
    _range_ite(int i, int s) : i(i), s(s) {}
    bool operator != (const _range_ite & o) const {
        return i!=o.i;
    }
    void operator ++ () {
        i += s;
    }
    const int & operator * () const {
        return i;
    }
};
struct _range_t {
    int b;
    int e;
    int s;
    _range_t(int b, int e, int s) : b(b), e(e), s(s) {}
    _range_ite begin() {
        return _range_ite(b, s);
    }
    _range_ite end() {
        return _range_ite(e, s);
    }
};
_range_t range(int i) {
    return _range_t(0, std::max(0, i), 1);
}
_range_t range(int b, int e) {
    return _range_t(b, std::max(b, e), 1);
}
_range_t range(int b, int e, int s) {
    assert(s!=0);
    if(0<s) {
        e = b + (std::max(0, e-b-1) / s + 1) * s;
    }
    else {
        e = b + (std::max(0, b-e-1) / -s + 1) * s;
    }
    return _range_t(b, e, s);
}
_range_t rrange(int i) {
    return _range_t(std::max(0, i)-1, -1, -1);
}
_range_t rrange(int b, int e) {
    return _range_t(std::max(b, e)-1, b-1, -1);
}
_range_t rrange(int b, int e, int s) {
    assert(s!=0);
    if(0<s) {
        e = b + (std::max(0, e-b-1) / s + 1) * s;
    }
    else {
        e = b + (std::max(0, b-e-1) / -s + 1) * s;
    }
    return _range_t(e-s, b-s, -s);
}
