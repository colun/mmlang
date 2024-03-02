class FileReader {
    FILE * file;
    char buf[65536];
    int bufLen;
    int bufPos;
    bool canReadFlag;
    bool crFlag;
    bool enterFlag;
    const char * inputCLineOrWord(int mode) {
        if(canReadFlag && (enterFlag ? bufLen<=bufPos : (int)sizeof(buf)<=bufPos+bufPos)) {
            if(0<bufLen-bufPos) {
                memmove(buf, buf+bufPos, bufLen-bufPos);
                bufLen -= bufPos;
            }
            else {
                bufLen = 0;
            }
            char * result = fgets(buf+bufLen, sizeof(buf)-bufLen, file);
            canReadFlag = (result!=NULL);
            if(result!=NULL) {
                int n = strlen(result);
                enterFlag = (n!=(int)sizeof(buf)-1-bufLen || (1<=bufLen+n && buf[bufLen+n-1]=='\n'));
                bufLen += n;
            }
            bufPos = 0;
        }
        if(mode==0) {
            int pos = bufPos;
            while(true) {
                char c = buf[pos];
                if(c==32) {
                    buf[pos++] = '\0';
                    break;
                }
                else if(c==10) {
                    if(crFlag) {
                        crFlag = false;
                        if(bufPos==pos) {
                            pos = ++bufPos;
                            continue;
                        }
                    }
                    buf[pos++] = '\0';
                    break;
                }
                else if(c==13) {
                    crFlag = true;
                    buf[pos++] = '\0';
                    break;
                }
                else if(c==0) {
                    break;
                }
                ++pos;
            }
            const char * ret = buf + bufPos;
            bufPos = pos;
            while(true) {
                char c = buf[bufPos];
                if(c==32 || c==10 || c==13) {
                    ++bufPos;
                }
                else {
                    break;
                }
            }
            return ret;
        }
        else if(mode==1) {
            int pos = bufPos;
            while(true) {
                char c = buf[pos];
                if(c==10) {
                    if(crFlag) {
                        crFlag = false;
                        if(bufPos==pos) {
                            pos = ++bufPos;
                            continue;
                        }
                    }
                    buf[pos++] = '\0';
                    break;
                }
                else if(c==13) {
                    crFlag = true;
                    buf[pos++] = '\0';
                    break;
                }
                else if(c==0) {
                    break;
                }
                ++pos;
            }
            const char * ret = buf + bufPos;
            bufPos = pos;
            if(crFlag) {
                while(true) {
                    char c = buf[bufPos];
                    if(c==13) {
                        ++bufPos;
                        crFlag = false;
                        break;
                    }
                    else {
                        break;
                    }
                }
            }
            return ret;
        }
        else if(mode==2) {
            return bufLen<=bufPos ? NULL : buf+bufPos;
        }
        assert(false);
        return NULL;
    }
public:
    FileReader() : file(NULL), bufLen(sizeof(buf)), bufPos(sizeof(buf)), canReadFlag(true), crFlag(false), enterFlag(false) {}
    FileReader(FILE * f) : file(f), bufLen(sizeof(buf)), bufPos(sizeof(buf)), canReadFlag(true), crFlag(false), enterFlag(false) {}
    void open(const string & fn) {
        if(file) {
            fclose(file);
        }
        file = fopen(fn.c_str(), "r");
        assert(file!=NULL);
        bufLen = sizeof(buf);
        bufPos = sizeof(buf);
        canReadFlag = true;
        crFlag = false;
        enterFlag = false;
    }
    void from(const string & data) {
        assert(false);
    }
    FileReader(const string & fn) : file(NULL), bufLen(sizeof(buf)), bufPos(sizeof(buf)), canReadFlag(true), crFlag(false), enterFlag(false) {
        open(fn);
    }
    const char * inputCLine() {
        return inputCLineOrWord(1);
    }
    const char * inputCWord() {
        return inputCLineOrWord(0);
    }
    std::string inputLine() {
        return inputCLine();
    }
    std::string inputWord() {
        return inputCWord();
    }
    double inputFloat() {
        return atof(inputCWord());
    }
    int inputInt() {
        return atoi(inputCWord());
    }
    long long inputInt64() {
        return atoll(inputCWord());
    }
    __int128 inputInt128() {
        const char * p = inputCWord();
        bool sign = false;
        if(*p=='-') {
            sign = true;
            ++p;
        }
        __int128 ret = 0;
        while('0'<=*p && *p<='9') {
            ret = ret * 10 + (*p - '0');
            ++p;
        }
        return sign ? -ret : ret;
    }
};
