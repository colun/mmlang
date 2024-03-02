template<class T>
class wrand49 {
    vector<tuple<double, T> > vec;
    vector<double> pvec;
    vector<T> vvec;
public:
    wrand49() {}
    void clear() {
        vec.clear();
    }
    void add(double p, const T & value) {
        vec.emplace_back(p, value);
    }
    void build() {
        sort(vec.begin(), vec.end());
        pvec.resize(vec.size());
        vvec.resize(vec.size());
        if(vec.size()) {
            pvec[0] = get<0>(vec[0]);
            vvec[0] = get<1>(vec[0]);
            for(int i=1; i<vec.size(); ++i) {
                pvec[i] = pvec[i-1] + get<0>(vec[i]);
                vvec[i] = get<1>(vec[i]);
            }
            double s = 1.0 / pvec.back();
            for(int i=0; i<vec.size()-1; ++i) {
                pvec[i] *= s;
            }
        }
    }
    const T & choice(double r) const {
        assert(0<=r);
        assert(r<=1);
        int b = 0;
        int e = pvec.size();
        while(2<=e-b) {
            int i = (b+e+1) >> 1;
            if(r<pvec[i-1]) {
                e = i;
            }
            else {
                b = i;
            }
        }
        return vvec[b];
    }
    const T & choice() const {
        return choice(drand49());
    }
};
