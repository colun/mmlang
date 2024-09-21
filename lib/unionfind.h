struct unionfind {
private:
    std::vector<std::tuple<int, int> > data;
public:
    unionfind() : data() {}
    void clear() {
        for(int i=0, e=data.size(); i<e; ++i) {
            data[i] = std::tuple<int, int>(i, 1);
        }
    }
    unionfind(int N) : data(N) {
        clear();
    }
    unionfind(unionfind & o) : data(o.data) {}
    void init(int N) {
        data.resize(N);
        clear();
    }
    int root(int i) {
        if(std::get<0>(data[i])==i) {
            return i;
        }
        return std::get<0>(data[i]) = root(std::get<0>(data[i]));
    }
    void unite(int u, int v) {
        u = root(u);
        v = root(v);
        if(u!=v) {
            if(std::get<1>(data[u])<std::get<1>(data[v])) {
                std::get<0>(data[u]) = v;
                std::get<1>(data[v]) += std::get<1>(data[u]);
            }
            else {
                std::get<0>(data[v]) = u;
                std::get<1>(data[u]) += std::get<1>(data[v]);
            }
        }
    }
};
