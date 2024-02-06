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
void gvPipe(const string & cmd) {
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
inline bool gvTime() {
    gvInit();
    fwrite("n\n", 2, 1, gv$file);
    gvFlush_();
    return true;
}
inline bool gvTime(double t) {
    gvInit();
    fprintf(gv$file, "n %g\n", t);
    gvFlush_();
    return true;
}
void gvRollback() {
    gvInit();
    fwrite("r\n", 2, 1, gv$file);
    gvFlush_();
}
void gvRollbackAll() {
    gvInit();
    fwrite("ra\n", 3, 1, gv$file);
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
    fprintf(gv$file, "c %g %g\n", x, y);
    gvFlush_();
}
void gvCircle(double x, double y, double r, gvRGB rgb) {
    gvInit();
    fprintf(gv$file, "c %g %g %u %g\n", x, y, rgb.toInt(), r);
    gvFlush_();
}
void gvCircle(double x, double y, double r) {
    gvCircle(x, y, r, gvRGB(0, 0, 0));
}
void gvCircle(double x, double y, gvRGB rgb) {
    gvInit();
    fprintf(gv$file, "c %g %g %u\n", x, y, rgb.toInt());
    gvFlush_();
}
void gvRect(double x, double y, double x2, double y2, double r, gvRGB rgb) {
    double bx = min(x, x2) - fabs(r);
    double by = min(y, y2) - fabs(r);
    double ex = max(x, x2) + fabs(r);
    double ey = max(y, y2) + fabs(r);
    gvInit();
    fprintf(gv$file, "p %u", rgb.toInt());
    fprintf(gv$file, " %g %g", bx, by);
    fprintf(gv$file, " %g %g", ex, by);
    fprintf(gv$file, " %g %g", ex, ey);
    fprintf(gv$file, " %g %g", bx, ey);
    fprintf(gv$file, "\n");
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
    gvInit();
    fprintf(gv$file, "t %g %g %u %g ", x, y, rgb.toInt(), r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvText(double x, double y, double r, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "t %g %g 0 %g ", x, y, r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvText(double x, double y, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "t %g %g %u 0.5 ", x, y, rgb.toInt());
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvText(double x, double y, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "t %g %g 0 0.5 ", x, y);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvTextLeft(double x, double y, double r, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tl %g %g %u %g ", x, y, rgb.toInt(), r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvTextLeft(double x, double y, double r, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tl %g %g 0 %g ", x, y, r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvTextLeft(double x, double y, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tl %g %g %u 0.5 ", x, y, rgb.toInt());
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvTextLeft(double x, double y, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tl %g %g 0 0.5 ", x, y);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvTextRight(double x, double y, double r, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tr %g %g %u %g ", x, y, rgb.toInt(), r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvTextRight(double x, double y, double r, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tr %g %g 0 %g ", x, y, r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvTextRight(double x, double y, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tr %g %g %u 0.5 ", x, y, rgb.toInt());
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvTextRight(double x, double y, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tr %g %g 0 0.5 ", x, y);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
void gvLine(double x1, double y1, double x2, double y2, double r, gvRGB rgb) {
    gvInit();
    fprintf(gv$file, "l %g %g %g %g %u %g\n", x1, y1, x2, y2, rgb.toInt(), r * 0.1);
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
    fprintf(gv$file, "la %g %g %g %g %u %g\n", x1, y1, x2, y2, rgb.toInt(), r * 0.1);
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
void gvLink(const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "il ");
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush_();
}
std::vector<string> gv$input_words;
int gv$input_cursor;
bool gvInputIsMessage() {
    assert(1<=gv$input_words.size());
    return gv$input_words[0].size()==1 && gv$input_words[0][0]=='k';
}
void gvInput() {
    gvInit();
    fprintf(gv$file, "i\n");
    gvFlush2_();
    char buf[4096];
    gv$input_words.clear();
    gv$input_words.emplace_back();
    std::string * word = &gv$input_words.back();
    while(true) {
        char * p = fgets(buf, sizeof(buf), gv$file_read);
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
const string & gvInputWord() {
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
    gvInit();
    fprintf(gv$file, "o ");
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
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
#endif
