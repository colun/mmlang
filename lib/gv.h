struct gvRGB {
    int a;
    int r;
    int g;
    int b;
    gvRGB() : a(0), r(0), g(0), b(0) {}
    gvRGB(int r, int g, int b, int a=0) : a(a), r(r), g(g), b(b) {
    }
    gvRGB(const std::vector<int> & rgb) : a(4<=rgb.size() ? rgb[3] : 0), r(rgb[0]), g(rgb[1]), b(rgb[2]) {
    }
    int toInt() const {
        return ((a & 255) << 24) | ((r & 255) << 16) | ((g & 255) << 8) | (b & 255);
    }
};
gvRGB gvHSB(double h, double s, double b) {
    assert(0<=s && s<=1.0);
    assert(0<=b && b<=1.0);
    h -= (int)h;
    if(h<0) {
        h += 1;
    }
    h *= 6;
    int h1 = (int)h;
    h -=  (int)h;
    double p = b * (1-s);
    double q = b * (1-s*h);
    double t = b * (1-s*(1-h));
    switch(h1) {
        case 0:
            return gvRGB((int)(b * 255 + 0.5), (int)(t * 255 + 0.5), (int)(p * 255 + 0.5));
        case 1:
            return gvRGB((int)(q * 255 + 0.5), (int)(b * 255 + 0.5), (int)(p * 255 + 0.5));
        case 2:
            return gvRGB((int)(p * 255 + 0.5), (int)(b * 255 + 0.5), (int)(t * 255 + 0.5));
        case 3:
            return gvRGB((int)(p * 255 + 0.5), (int)(q * 255 + 0.5), (int)(b * 255 + 0.5));
        case 4:
            return gvRGB((int)(t * 255 + 0.5), (int)(p * 255 + 0.5), (int)(b * 255 + 0.5));
    }
    return gvRGB((int)(b * 255 + 0.5), (int)(p * 255 + 0.5), (int)(q * 255 + 0.5));
}
#ifdef MM$VIS
char gv$buf[65536];
std::vector<std::string> gv$input_words;
int gv$input_cursor;
bool gvInputIsMessage() {
    assert(1<=gv$input_words.size());
    return gv$input_words[0].size()==1 && gv$input_words[0][0]=='k';
}
#if __emscripten__
EM_JS(void, gvSendJs, (const char * line, int len), {
    var v = UTF8ToString(line, len);
    gv.addLine(v);
});
EM_JS(void, gvRecv, (char * buf, int len), {
    stringToUTF8(gvInputLine, buf, len);
});
#define gvInit(...)
#define gvPipe(...)
bool gv$flushFlag = true;
void gvFlush() {
    gv$flushFlag = false;
    //TODO
}
void gvFlush2_() {
    //TODO
}
void gvFlush_() {
    if(gv$flushFlag) {
        //TODO
    }
}
char gv$sendfBuf[65536];
void gvSend(const char * line) {
    gvSendJs(line, strlen(line));
}
void gvSendf(const char * format, ...) {
    va_list arg;
    va_start(arg, format);
    int len = vsprintf(gv$sendfBuf, format, arg);
    va_end(arg);
    gvSendJs(gv$sendfBuf, len);
}
void gvInput() {
    gvInit();
    gvSend("i");
    gvFlush2_();
    mm$suspend_mm_fiber();
    gv$input_words.clear();
    gv$input_words.emplace_back();
    std::string * word = &gv$input_words.back();
    while(true) {
        gvRecv(gv$buf, sizeof(gv$buf));
        const char * p = gv$buf;
        if(!p) {
            break;
        }
        char c = 0;
        for(const char * p2 = p; ; ++p2) {
            c = *p2;
            if(!c || c=='\n') {
                break;
            }
            else if(c==' ') {
                gv$input_words.emplace_back();
                word = &gv$input_words.back();
            }
            else {
                word->push_back(c);
            }
        }
        if(c=='\n') {
            break;
        }
    }
    if(word->empty() && 2<=gv$input_words.size()) {
        gv$input_words.pop_back();
    }
    gv$input_cursor = gvInputIsMessage() ? 1 : 0;
}
#else
void popen2(const char * cmd, FILE * & w, FILE * & r) {
    int stdin_pipe[2];
    int stdout_pipe[2];
    {
        int err = pipe(stdin_pipe);
        assert(err==0);
    }
    {
        int err = pipe(stdout_pipe);
        assert(err==0);
    }
    pid_t p = fork();
    assert(0<=p);
    if(p==0) {
        close(stdin_pipe[1]);
        close(stdout_pipe[0]);
        dup2(stdin_pipe[0], 0);
        dup2(stdout_pipe[1], 1);
        std::vector<std::string> v;
        {
            const char * st = cmd;
            const char * en = cmd;
            for(const char * en = cmd; ; ++en) {
                char ch = *en;
                if(ch==' ' || ch==0) {
                    if(st!=en) {
                        v.emplace_back(st, en);
                    }
                    if(ch==0) {
                        break;
                    }
                    ++en;
                    st = en;
                }
            }
        }
        std::vector<const char *> v2;
        for(const std::string & s : v) {
            v2.emplace_back(s.c_str());
        }
        v2.emplace_back((const char*)NULL);
        assert(2<=v2.size());
        execvp(v2[0], (char**)v2.data());
        assert(false);
    }
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    w = fdopen(stdin_pipe[1], "w");
    r = fdopen(stdout_pipe[0], "r");
}
FILE * gv$file = NULL;
FILE * gv$file_read = NULL;
bool gv$flushFlag = true;
void gvInit() {
    if(gv$file==NULL) {
        gv$file = fopen("result.gv", "w+");
    }
}
void gvPipe(const std::string & cmd) {
    if(gv$file==NULL) {
        popen2(cmd.c_str(), gv$file, gv$file_read);
        fprintf(stderr, "popen %s ... %p %p\n", cmd.c_str(), gv$file, gv$file_read);
    }
}
void gvFlush() {
    gv$flushFlag = false;
    if(gv$file) {

        fwrite("f\n", 2, 1, gv$file);
        fflush(gv$file);
    }
}
void gvFlush2_() {
    if(gv$file) {
        fwrite("f\n", 2, 1, gv$file);
        fflush(gv$file);
    }
}
void gvFlush_() {
    if(gv$flushFlag) {
        assert(gv$file);
        fwrite("f\n", 2, 1, gv$file);
        fflush(gv$file);
    }
}
void gvSend(const char * line) {
    fprintf(gv$file, "%s\n", line);
}
void gvSendf(const char * format, ...) {
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fputc('\n', gv$file);
}
void gvInput() {
    gvInit();
    gvSend("i");
    gvFlush2_();
    gv$input_words.clear();
    gv$input_words.emplace_back();
    std::string * word = &gv$input_words.back();
    while(true) {
        char * p = fgets(gv$buf, sizeof(gv$buf), gv$file_read);
        if(!p) {
            break;
        }
        char c = 0;
        for(char * p2 = p; ; ++p2) {
            c = *p2;
            if(!c || c=='\n') {
                break;
            }
            else if(c==' ') {
                gv$input_words.emplace_back();
                word = &gv$input_words.back();
            }
            else {
                word->push_back(c);
            }
        }
        if(c=='\n') {
            break;
        }
    }
    if(word->empty() && 2<=gv$input_words.size()) {
        gv$input_words.pop_back();
    }
    gv$input_cursor = gvInputIsMessage() ? 1 : 0;
}
#endif
inline bool gvTime() {
    gvInit();
    gvSend("n");
    gvFlush_();
    return true;
}
inline bool gvTime(double t) {
    gvInit();
    gvSendf("n %g", t);
    gvFlush_();
    return true;
}
void gvRollback() {
    gvInit();
    gvSend("r");
    gvFlush_();
}
void gvRollbackAll() {
    gvInit();
    gvSend("ra");
    gvFlush_();
}
double gv$fps = -1000000000.0;
inline bool gvFps(double t, double fps=60.0) {
    if(gv$fps<=t) {
        gv$fps = t + 1.0 / fps;
        gvTime(t);
        return true;
    }
    return false;
}
inline bool gvFps() {
    return gvFps(getTime());
}
inline bool gvFpsWithRollback(double t, double fps=60.0) {
    if(gv$fps<=t) {
        gv$fps = t + 1.0 / fps;
        gvRollback();
        gvTime(t);
        return true;
    }
    return false;
}
inline bool gvFpsWithRollback() {
    return gvFpsWithRollback(getTime());
}
void gvCircle(double x, double y) {
    gvInit();
    gvSendf("c %g %g", x, y);
    gvFlush_();
}
void gvCircle(double x, double y, double r, gvRGB rgb) {
    gvInit();
    gvSendf("c %g %g %u %g", x, y, rgb.toInt(), r);
    gvFlush_();
}
void gvCircle(double x, double y, double r) {
    gvCircle(x, y, r, gvRGB(0, 0, 0));
}
void gvCircle(double x, double y, gvRGB rgb) {
    gvInit();
    gvSendf("c %g %g %u\n", x, y, rgb.toInt());
    gvFlush_();
}
void gvRect(double x, double y, double x2, double y2, double r, gvRGB rgb) {
    double bx = min(x, x2) - fabs(r);
    double by = min(y, y2) - fabs(r);
    double ex = max(x, x2) + fabs(r);
    double ey = max(y, y2) + fabs(r);
    gvInit();
    gvSendf("p %u %g %g %g %g %g %g %g %g", rgb.toInt(), bx, by, ex, by, ex, ey, bx, ey);
    gvFlush_();
}
void gvRect(double x, double y, double x2, double y2, gvRGB rgb) {
    gvRect(x, y, x2, y2, 0.5, rgb);
}
void gvRect(double x, double y, double x2, double y2, double r) {
    gvRect(x, y, x2, y2, r, gvRGB(0, 0, 0));
}
void gvRect(double x, double y, double x2, double y2) {
    gvRect(x, y, x2, y2, 0.5, gvRGB(0, 0, 0));
}
void gvRect(double x, double y, double r, gvRGB rgb) {
    gvRect(x, y, x, y, r, rgb);
}
void gvRect(double x, double y, double r) {
    gvRect(x, y, x, y, r, gvRGB(0, 0, 0));
}
void gvRect(double x, double y, gvRGB rgb) {
    gvRect(x, y, x, y, 0.5, rgb);
}
void gvRect(double x, double y) {
    gvRect(x, y, x, y, 0.5, gvRGB(0, 0, 0));
}
void gvText(double x, double y, double r, gvRGB rgb, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("t %g %g %u %g %s", x, y, rgb.toInt(), r, gv$buf);
    gvFlush_();
}
void gvText(double x, double y, double r, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("t %g %g 0 %g %s", x, y, r, gv$buf);
    gvFlush_();
}
void gvText(double x, double y, gvRGB rgb, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("t %g %g %u 0.5 %s", x, y, rgb.toInt(), gv$buf);
    gvFlush_();
}
void gvText(double x, double y, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("t %g %g 0 0.5 %s", x, y, gv$buf);
    gvFlush_();
}
void gvTextLeft(double x, double y, double r, gvRGB rgb, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("tl %g %g %u %g %s", x, y, rgb.toInt(), r, gv$buf);
    gvFlush_();
}
void gvTextLeft(double x, double y, double r, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("tl %g %g 0 %g %s", x, y, r, gv$buf);
    gvFlush_();
}
void gvTextLeft(double x, double y, gvRGB rgb, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("tl %g %g %u 0.5 %s", x, y, rgb.toInt(), gv$buf);
    gvFlush_();
}
void gvTextLeft(double x, double y, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("tl %g %g 0 0.5 %s", x, y, gv$buf);
    gvFlush_();
}
void gvTextRight(double x, double y, double r, gvRGB rgb, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("tr %g %g %u %g %s", x, y, rgb.toInt(), r, gv$buf);
    gvFlush_();
}
void gvTextRight(double x, double y, double r, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("tr %g %g 0 %g %s", x, y, r, gv$buf);
    gvFlush_();
}
void gvTextRight(double x, double y, gvRGB rgb, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("tr %g %g %u 0.5 %s", x, y, rgb.toInt(), gv$buf);
    gvFlush_();
}
void gvTextRight(double x, double y, const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("tr %g %g 0 0.5 %s", x, y, gv$buf);
    gvFlush_();
}
void gvLine(double x1, double y1, double x2, double y2, double r, gvRGB rgb) {
    gvInit();
    gvSendf("l %g %g %g %g %u %g\n", x1, y1, x2, y2, rgb.toInt(), r * 0.1);
    gvFlush_();
}
void gvLine(double x1, double y1, double x2, double y2, double r) {
    gvLine(x1, y1, x2, y2, r, gvRGB(0, 0, 0));
}
void gvLine(double x1, double y1, double x2, double y2, gvRGB rgb) {
    gvLine(x1, y1, x2, y2, 0.5, rgb);
}
void gvLine(double x1, double y1, double x2, double y2) {
    gvLine(x1, y1, x2, y2, 0.5);
}
void gvArrow(double x1, double y1, double x2, double y2, double r, gvRGB rgb) {
    gvInit();
    gvSendf("la %g %g %g %g %u %g\n", x1, y1, x2, y2, rgb.toInt(), r * 0.1);
    gvFlush_();
}
void gvArrow(double x1, double y1, double x2, double y2, double r) {
    gvArrow(x1, y1, x2, y2, r, gvRGB(0, 0, 0));
}
void gvArrow(double x1, double y1, double x2, double y2, gvRGB rgb) {
    gvArrow(x1, y1, x2, y2, 0.5, rgb);
}
void gvArrow(double x1, double y1, double x2, double y2) {
    gvArrow(x1, y1, x2, y2, 0.5);
}
void gvShogi(double x1, double y1, double x2, double y2, double r, gvRGB rgb) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dd = dx * dx + dy * dy;
    if(dd<0.0000000000000001) {
        gvCircle(x1, y1, r, rgb);
        return;
    }
    double d = sqrt(dd);
    double rate = r / d;
    dx *= rate;
    dy *= rate;
    double dx52 = dx * 0.52;
    double dy52 = dy * 0.52;
    double dx66 = dx * 0.66;
    double dy66 = dy * 0.66;
    double dx75 = dx * 0.75;
    double dy75 = dy * 0.75;
    double dx84 = dx * 0.84;
    double dy84 = dy * 0.84;
    gvInit();
    gvSendf("p %u %g %g %g %g %g %g %g %g %g %g", rgb.toInt(), x1 + dx, y1 + dy, x1 + dx84 + dy52, y1 + dy84 - dx52, x1 - dx66 + dy75, y1 - dy66 - dx75, x1 - dx66 - dy75, y1 - dy66 + dx75, x1 + dx84 - dy52, y1 + dy84 + dx52);
    gvFlush_();
}
void gvShogi(double x1, double y1, double x2, double y2, double r) {
    gvShogi(x1, y1, x2, y2, r, gvRGB(0, 0, 0));
}
void gvShogi(double x1, double y1, double x2, double y2, gvRGB rgb) {
    gvShogi(x1, y1, x2, y2, 0.5, rgb);
}
void gvShogi(double x1, double y1, double x2, double y2) {
    gvShogi(x1, y1, x2, y2, 0.5, gvRGB(0, 0, 0));
}
void gvShogi(double x, double y, double r, gvRGB rgb) {
    gvShogi(x, y, x, y-1, r, rgb);
}
void gvShogi(double x, double y, double r) {
    gvShogi(x, y, x, y-1, r, gvRGB(0, 0, 0));
}
void gvShogi(double x, double y, gvRGB rgb) {
    gvShogi(x, y, x, y-1, 0.5, rgb);
}
void gvShogi(double x, double y) {
    gvShogi(x, y, x, y-1, 0.5, gvRGB(0, 0, 0));
}
void gvLink(const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("il %s", gv$buf);
    gvFlush_();
}
void gvKeyboard(const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("ik %s", gv$buf);
    gvFlush_();
}
const std::string & gvInputWord() {
    assert(gv$input_cursor<gv$input_words.size());
    return gv$input_words[gv$input_cursor++];
}
int gvInputInt() {
    return atoi(gvInputWord().c_str());
}
double gvInputFloat() {
    return atof(gvInputWord().c_str());
}
void gvOutput(const char * format = "?", ...) {
    va_list arg;
    va_start(arg, format);
    vsprintf(gv$buf, format, arg);
    va_end(arg);
    gvInit();
    gvSendf("o %s", gv$buf);
    gvFlush_();
}
#else
#define gvPipe(...)
#define gvFlush(...)
#define gvFps(...) false
#define gvTime(...) false
#define gvCircle(...)
#define gvRect(...)
#define gvText(...)
#define gvTextLeft(...)
#define gvLine(...)
#define gvArrow(...)
#define gvRollback(...)
#define gvRollbackAll(...)
#define gvOutput(...)
#define gvLink(...)
#define gvInput(...)
#define gvKeyboard(...)
#define gvInputWord(...) ""
#define gvInputInt(...) 0
#define gvInputFloat(...) 0.0
#endif
