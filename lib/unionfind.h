struct unionfind {
private:
    int N;
    vector<int> parent;
    vector<int> count;
public:
    unionfind() : N(0), parent(), count() {}
    unionfind(int N) : N(N), parent(N), count(N, 1) {
        for(int i=0, e=N; i<e; ++i) {
            parent[i] = i;
        }
    }
    unionfind(unionfind & o) : N(o.N), parent(o.parent), count(o.count) {}
    void clear() {
        for(int i=0, e=N; i<e; ++i) {
            parent[i] = i;
            count[i] = 1;
        }
    }
    int root(int i) {
        if(parent[i]==i) {
            return i;
        }
        return parent[i] = root(parent[i]);
    }
    void unite(int u, int v) {
        u = root(u);
        v = root(v);
        if(u!=v) {
            if(count[u]<count[v]) {
                parent[u] = v;
                count[v] += count[u];
            }
            else {
                parent[v] = u;
                count[u] += count[v];
            }
        }
    }
};
