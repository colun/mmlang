const char * inputCLineOrWord(int mode) {
    static char buf[2097152];
    static int bufLen = sizeof(buf);
    static int bufPos = sizeof(buf);
    static bool canReadFlag = true;
    static bool crFlag = false;
    static bool enterFlag = false;
    if(canReadFlag && (enterFlag ? bufLen<=bufPos : (int)sizeof(buf)<=bufPos+bufPos)) {
        if(0<bufLen-bufPos) {
            memmove(buf, buf+bufPos, bufLen-bufPos);
            bufLen -= bufPos;
        }
        else {
            bufLen = 0;
        }
        char * result = fgets(buf+bufLen, sizeof(buf)-bufLen, stdin);
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
