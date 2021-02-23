template<class It> class _slice_t {
private:
    It b;
    It e;
public:
    _slice_t(It b, It e) : b(b), e(e) {}
    It begin() {
        return b;
    }
    It end() {
        return e;
    }
};

template<class It> _slice_t<It> _slice(It b, It e) {
    return _slice_t<It>(b, e);
}
