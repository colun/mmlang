FILE * store2d$file = NULL;
template<class T> void store2d(addresser2d & I, const T & X, const std::vector<char> & Y) {
    assert(I.fin()<=X.size());
    assert(I.fin()<=Y.size());
    char buf[1024];
    if(!store2d$file) {
        mkdir("store", 0777);
        sprintf(buf, "store/%s.csv", paramStr("--seed", "0").c_str());
        store2d$file = fopen(buf, "w");
        assert(store2d$file);
    }
    const int channels = X[0].size();
    fprintf(store2d$file, "%d,%d,%d,", I.height(), I.width(), channels);
    char * p = buf;
    int pos = I.begin();
    for(int y=0; y<I.height(); ++y) {
        int pos_x = pos;
        for(int x=0; x<I.width(); ++x) {
            for(int c=0; c<channels; ++c) {
                encode_float18(p, X[pos_x][c]);
                if(buf+1000<=p) {
                    fwrite(buf, 1, p-buf, store2d$file);
                    p = buf;
                }
            }
            ++pos_x;
        }
        pos += I.stride();
    }
    *p = ',';
    ++p;
    pos = I.begin();
    for(int y=0; y<I.height(); ++y) {
        int pos_x = pos;
        for(int x=0; x<I.width(); ++x) {
            int v = Y[pos_x] + 1;
            assert(0<=v && v<64);
            *p = encode_float18$base64[v&63];
            ++p;
            if(buf+1000<=p) {
                fwrite(buf, 1, p-buf, store2d$file);
                p = buf;
            }
            ++pos_x;
        }
        pos += I.stride();
    }
    *p = '\n';
    ++p;
    fwrite(buf, 1, p-buf, store2d$file);
    fflush(store2d$file);
}
