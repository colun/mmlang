template<int P=1000000007> class modint {
    int value_;
public:
    modint() noexcept : value_(0) {}
    constexpr modint(int n) noexcept : value_(n) {
        assert(0<=n && n<P);
    }
    operator int() const {
        return value_;
    }
    modint<P> operator + (const modint<P> & o) const noexcept {
        int r = value_ + o.value_;
        if(P<=r) {
            r -= P;
        }
        return modint<P>(r);
    }
    modint<P> operator - (const modint<P> & o) const noexcept {
        int r = value_ - o.value_;
        if(r<0) {
            r += P;
        }
        return modint<P>(r);
    }
    modint<P> operator * (const modint<P> & o) const noexcept {
        return modint<P>(mod_mul(value_, o.value_, P));
    }
    modint<P> & operator += (const modint<P> & o) noexcept {
        value_ += o.value_;
        if(P<=value_) {
            value_ -= P;
        }
        return *this;
    }
    modint<P> & operator -= (const modint<P> & o) noexcept {
        value_ -= o.value_;
        if(value_<0) {
            value_ += P;
        }
        return *this;
    }
    modint<P> & operator *= (const modint<P> & o) noexcept {
        value_ = mod_mul(value_, o.value_, P);
        return *this;
    }
};
