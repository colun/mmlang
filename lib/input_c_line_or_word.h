char inputCLineOrWord$buf[2097152];
int inputCLineOrWord$bufLen = sizeof(inputCLineOrWord$buf);
int inputCLineOrWord$bufPos = sizeof(inputCLineOrWord$buf);
bool inputCLineOrWord$canReadFlag = true;
bool inputCLineOrWord$crFlag = false;
bool inputCLineOrWord$enterFlag = false;
FILE * inputCLineOrWord$stdin = stdin;

void inputOpen(const string & fn) {
    if(inputCLineOrWord$stdin!=stdin) {
        fclose(inputCLineOrWord$stdin);
        inputCLineOrWord$stdin = stdin;
    }
    inputCLineOrWord$stdin = fopen(fn.c_str(), "r");
    inputCLineOrWord$bufLen = sizeof(inputCLineOrWord$buf);
    inputCLineOrWord$bufPos = sizeof(inputCLineOrWord$buf);
    inputCLineOrWord$canReadFlag = true;
    inputCLineOrWord$crFlag = false;
    inputCLineOrWord$enterFlag = false;
}

//void inputRollback() {
//
//}

const char * inputCLineOrWord(int mode) {
    if(inputCLineOrWord$canReadFlag && (inputCLineOrWord$enterFlag ? inputCLineOrWord$bufLen<=inputCLineOrWord$bufPos : (int)sizeof(inputCLineOrWord$buf)<=inputCLineOrWord$bufPos+inputCLineOrWord$bufPos)) {
        if(0<inputCLineOrWord$bufLen-inputCLineOrWord$bufPos) {
            memmove(inputCLineOrWord$buf, inputCLineOrWord$buf+inputCLineOrWord$bufPos, inputCLineOrWord$bufLen-inputCLineOrWord$bufPos);
            inputCLineOrWord$bufLen -= inputCLineOrWord$bufPos;
        }
        else {
            inputCLineOrWord$bufLen = 0;
        }
        char * result = fgets(inputCLineOrWord$buf+inputCLineOrWord$bufLen, sizeof(inputCLineOrWord$buf)-inputCLineOrWord$bufLen, inputCLineOrWord$stdin);
        inputCLineOrWord$canReadFlag = (result!=NULL);
        if(result!=NULL) {
            int n = strlen(result);
            inputCLineOrWord$enterFlag = (n!=(int)sizeof(inputCLineOrWord$buf)-1-inputCLineOrWord$bufLen || (1<=inputCLineOrWord$bufLen+n && inputCLineOrWord$buf[inputCLineOrWord$bufLen+n-1]=='\n'));
            inputCLineOrWord$bufLen += n;
        }
        inputCLineOrWord$bufPos = 0;
    }
    if(inputCLineOrWord$bufLen==inputCLineOrWord$bufPos) {
        return "";
    }
    if(mode==0) {
        int pos = inputCLineOrWord$bufPos;
        while(true) {
            char c = inputCLineOrWord$buf[pos];
            if(c==32) {
                inputCLineOrWord$buf[pos++] = '\0';
                break;
            }
            else if(c==10) {
                if(inputCLineOrWord$crFlag) {
                    inputCLineOrWord$crFlag = false;
                    if(inputCLineOrWord$bufPos==pos) {
                        pos = ++inputCLineOrWord$bufPos;
                        continue;
                    }
                }
                inputCLineOrWord$buf[pos++] = '\0';
                break;
            }
            else if(c==13) {
                inputCLineOrWord$crFlag = true;
                inputCLineOrWord$buf[pos++] = '\0';
                break;
            }
            else if(c==0) {
                break;
            }
            ++pos;
        }
        const char * ret = inputCLineOrWord$buf + inputCLineOrWord$bufPos;
        inputCLineOrWord$bufPos = pos;
        while(true) {
            char c = inputCLineOrWord$buf[inputCLineOrWord$bufPos];
            if(c==32 || c==10 || c==13) {
                ++inputCLineOrWord$bufPos;
            }
            else {
                break;
            }
        }
        return ret;
    }
    else if(mode==1) {
        int pos = inputCLineOrWord$bufPos;
        while(true) {
            char c = inputCLineOrWord$buf[pos];
            if(c==10) {
                if(inputCLineOrWord$crFlag) {
                    inputCLineOrWord$crFlag = false;
                    if(inputCLineOrWord$bufPos==pos) {
                        pos = ++inputCLineOrWord$bufPos;
                        continue;
                    }
                }
                inputCLineOrWord$buf[pos++] = '\0';
                break;
            }
            else if(c==13) {
                inputCLineOrWord$crFlag = true;
                inputCLineOrWord$buf[pos++] = '\0';
                break;
            }
            else if(c==0) {
                break;
            }
            ++pos;
        }
        const char * ret = inputCLineOrWord$buf + inputCLineOrWord$bufPos;
        inputCLineOrWord$bufPos = pos;
        if(inputCLineOrWord$crFlag) {
            while(true) {
                char c = inputCLineOrWord$buf[inputCLineOrWord$bufPos];
                if(c==13) {
                    ++inputCLineOrWord$bufPos;
                    inputCLineOrWord$crFlag = false;
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
        return inputCLineOrWord$bufLen<=inputCLineOrWord$bufPos ? "" : inputCLineOrWord$buf+inputCLineOrWord$bufPos;
    }
    assert(false);
    return "";
}
