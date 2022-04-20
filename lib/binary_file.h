class BinaryFile {
private:
    FILE * file;
    long long offset;
    int bpos;
    int epos;
    char buf[8192];
    void prepare(int needs) {
        assert(file);
        assert(needs<=4096);
        int len = epos-bpos;
        if(len<needs) {
            std::memmove(buf, buf+bpos, len);
            int ret = std::fread(buf+len, 1, 4096, file);
            assert(0<ret);
            offset += ret;
            epos = len + ret;
            bpos = 0;
            assert(needs<=epos);
        }
    }
public:
    BinaryFile() : file(NULL), offset(0), bpos(0), epos(0) {}
    void close() {
        if(file) {
            std::fclose(file);
            file = NULL;
        }
    }
    ~BinaryFile() {
        close();
    }
    bool open(const std::string & fn) {
        close();
        file = std::fopen(fn.c_str(), "rb");
        offset = 0;
        bpos = 0;
        epos = 0;
        return file!=NULL;
    }
    void skip(int n) {
        int bpos2 = bpos + n;
        if(0<=bpos2 && bpos2<=epos) {
            bpos = bpos2;
            return;
        }
        offset += bpos - epos;
        offset += n;
        fseeko64(file, offset, SEEK_SET);
        bpos = 0;
        epos = 0;
    }
    void skip(long long n) {
        long long bpos2 = bpos + n;
        if(0<=bpos2 && bpos2<=epos) {
            bpos = (int)bpos2;
            return;
        }
        offset += bpos2 - epos;
        fseeko64(file, offset, SEEK_SET);
        bpos = 0;
        epos = 0;
    }
    void seek(int n) {
        long long bpos2 = n - (offset - epos);
        if(0<=bpos2 && bpos2<=epos) {
            bpos = (int)bpos2;
            return;
        }
        std::fseek(file, n, SEEK_SET);
        offset = n;
        bpos = 0;
        epos = 0;
    }
    void seek(long long n) {
        long long bpos2 = n - (offset - epos);
        if(0<=bpos2 && bpos2<=epos) {
            bpos = (int)bpos2;
            return;
        }
        fseeko64(file, n, SEEK_SET);
        offset = n;
        bpos = 0;
        epos = 0;
    }
    char inputInt8() {
        prepare(1);
        char ret = *(char*)(buf + bpos);
        bpos += 1;
        return ret;
    }
    short inputInt16() {
        prepare(2);
        short ret = *(short*)(buf + bpos);
        bpos += 2;
        return ret;
    }
    int inputInt32() {
        prepare(4);
        int ret = *(int*)(buf + bpos);
        bpos += 4;
        return ret;
    }
    int inputInt() {
        return inputInt32();
    }
    long long inputInt64() {
        prepare(8);
        int ret = *(long long*)(buf + bpos);
        bpos += 8;
        return ret;
    }
    float inputFloat32() {
        prepare(4);
        float ret = *(float*)(buf + bpos);
        bpos += 4;
        return ret;
    }
    double inputFloat64() {
        prepare(8);
        double ret = *(double*)(buf + bpos);
        bpos += 8;
        return ret;
    }
    double inputFloat() {
        return inputFloat64();
    }
};
