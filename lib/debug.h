void debug_unit() {
}
void debug_unit(bool val) {
    fprintf(stderr, "%s", val ? "true" : "false");
}
void debug_unit(char val) {
    fprintf(stderr, "%c", val);
}
void debug_unit(int val) {
    fprintf(stderr, "%d", val);
}
void debug_unit(unsigned int val) {
    fprintf(stderr, "%u", val);
}
void debug_unit(size_t val) {
    fprintf(stderr, "%zd", val);
}
void debug_unit(const void * val) {
    fprintf(stderr, "%p", val);
}
void debug_unit(long long val) {
    fprintf(stderr, "%lld", val);
}
void debug_unit(__int128 val) {
    char buf[128];
    int idx = 128;
    buf[--idx] = '\0';
    bool sign = false;
    if(val<0) {
        sign = true;
        val = -val;
    }
    if(val==0) {
        buf[--idx] = '0';
    }
    else while(val) {
        buf[--idx] = '0' + (val % 10);
        val /= 10;
    }
    if(sign) {
        buf[--idx] = '-';
    }
    fprintf(stderr, "%s", buf+idx);
}
void debug_unit(double val) {
    fprintf(stderr, "%g", val);
}
void debug_unit(const char * val) {
    fprintf(stderr, "%s", val);
}
void debug_unit(const std::string & val) {
    fprintf(stderr, "%s", val.c_str());
}
template<class T>void debug_unit(const std::vector<T> & val) {
    fprintf(stderr, "vec(%d) {", (int)val.size());
    for(int i=0; i<(int)val.size(); ++i) {
        fputc(' ', stderr);
        debug_unit(val[i]);
    }
    fprintf(stderr, " }");
}

#define _debug1(e) debug_unit(e)
#define _debug2(e1, e2) _debug1(e1),fputc(' ', stderr),_debug1(e2)
#define _debug3(e1, e2, e3) _debug2(e1, e2),fputc(' ', stderr),_debug1(e3)
#define _debug4(e1, e2, e3, e4) _debug2(e1, e2),fputc(' ', stderr),_debug2(e3, e4)
#define _debug5(e1, e2, e3, e4, e5) _debug3(e1, e2, e3),fputc(' ', stderr),_debug2(e4, e5)
#define _debug6(e1, e2, e3, e4, e5, e6) _debug3(e1, e2, e3),fputc(' ', stderr),_debug3(e4, e5, e6)
#define _debug7(e1, e2, e3, e4, e5, e6, e7) _debug4(e1, e2, e3, e4),fputc(' ', stderr),_debug3(e5, e6, e7)
#define _debug8(e1, e2, e3, e4, e5, e6, e7, e8) _debug4(e1, e2, e3, e4),fputc(' ', stderr),_debug4(e5, e6, e7, e8)
#define _debug9(e1, e2, e3, e4, e5, e6, e7, e8, e9) _debug5(e1, e2, e3, e4, e5),fputc(' ', stderr),_debug4(e6, e7, e8, e9)
#define _debug10(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) _debug5(e1, e2, e3, e4, e5),fputc(' ', stderr),_debug5(e6, e7, e8, e9, e10)
#define _GET_PRINT_MACRO_NAME(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define debug(...) _GET_PRINT_MACRO_NAME(__VA_ARGS__, _debug10, _debug9, _debug8, _debug7, _debug6, _debug5, _debug4, _debug3, _debug2, _debug1)(__VA_ARGS__),fputc('\n', stderr)
#define debug0(...) _GET_PRINT_MACRO_NAME(__VA_ARGS__, _debug10, _debug9, _debug8, _debug7, _debug6, _debug5, _debug4, _debug3, _debug2, _debug1)(__VA_ARGS__)
#define debugF(...) _GET_PRINT_MACRO_NAME(__VA_ARGS__, _debug10, _debug9, _debug8, _debug7, _debug6, _debug5, _debug4, _debug3, _debug2, _debug1)(__VA_ARGS__),fputc('\n', stderr),fflush(stderr)
#define debug0F(...) _GET_PRINT_MACRO_NAME(__VA_ARGS__, _debug10, _debug9, _debug8, _debug7, _debug6, _debug5, _debug4, _debug3, _debug2, _debug1)(__VA_ARGS__),fflush(stderr)
