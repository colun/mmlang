void print_unit() {
}
void print_unit(bool val) {
    printf("%s", val ? "true" : "false");
}
void print_unit(char val) {
    printf("%c", val);
}
void print_unit(int val) {
    printf("%d", val);
}
void print_unit(unsigned int val) {
    printf("%u", val);
}
void print_unit(size_t val) {
    printf("%zd", val);
}
void print_unit(const void * val) {
    printf("%p", val);
}
void print_unit(long long val) {
    printf("%lld", val);
}
void print_unit(__int128 val) {
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
    printf("%s", buf+idx);
}
void print_unit(double val) {
    printf("%g", val);
}
void print_unit(const char * val) {
    printf("%s", val);
}
void print_unit(const std::string & val) {
    printf("%s", val.c_str());
}
template<class T>void print_unit(const std::vector<T> & val) {
    printf("vec(%d) {", (int)val.size());
    for(int i=0; i<(int)val.size(); ++i) {
        fputc(' ', stdout);
        print_unit(val[i]);
    }
    printf(" }");
}

#define _print1(e) print_unit(e)
#define _print2(e1, e2) _print1(e1),fputc(' ', stdout),_print1(e2)
#define _print3(e1, e2, e3) _print2(e1, e2),fputc(' ', stdout),_print1(e3)
#define _print4(e1, e2, e3, e4) _print2(e1, e2),fputc(' ', stdout),_print2(e3, e4)
#define _print5(e1, e2, e3, e4, e5) _print3(e1, e2, e3),fputc(' ', stdout),_print2(e4, e5)
#define _print6(e1, e2, e3, e4, e5, e6) _print3(e1, e2, e3),fputc(' ', stdout),_print3(e4, e5, e6)
#define _print7(e1, e2, e3, e4, e5, e6, e7) _print4(e1, e2, e3, e4),fputc(' ', stdout),_print3(e5, e6, e7)
#define _print8(e1, e2, e3, e4, e5, e6, e7, e8) _print4(e1, e2, e3, e4),fputc(' ', stdout),_print4(e5, e6, e7, e8)
#define _print9(e1, e2, e3, e4, e5, e6, e7, e8, e9) _print5(e1, e2, e3, e4, e5),fputc(' ', stdout),_print4(e6, e7, e8, e9)
#define _print10(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) _print5(e1, e2, e3, e4, e5),fputc(' ', stdout),_print5(e6, e7, e8, e9, e10)
#define _print11(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11) _print6(e1, e2, e3, e4, e5, e6),fputc(' ', stdout),_print5(e7, e8, e9, e10, e11)
#define _print12(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12) _print6(e1, e2, e3, e4, e5, e6),fputc(' ', stdout),_print6(e7, e8, e9, e10, e11, e12)
#define _print13(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13) _print7(e1, e2, e3, e4, e5, e6, e7),fputc(' ', stdout),_print6(e8, e9, e10, e11, e12, e13)
#define _print14(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14) _print7(e1, e2, e3, e4, e5, e6, e7),fputc(' ', stdout),_print7(e8, e9, e10, e11, e12, e13, e14)
#define _print15(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15) _print8(e1, e2, e3, e4, e5, e6, e7, e8),fputc(' ', stdout),_print7(e9, e10, e11, e12, e13, e14, e15)
#define _print16(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16) _print8(e1, e2, e3, e4, e5, e6, e7, e8),fputc(' ', stdout),_print8(e9, e10, e11, e12, e13, e14, e15, e16)
#define _print17(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17) _print9(e1, e2, e3, e4, e5, e6, e7, e8, e9),fputc(' ', stdout),_print8(e10, e11, e12, e13, e14, e15, e16, e17)
#define _print18(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18) _print9(e1, e2, e3, e4, e5, e6, e7, e8, e9),fputc(' ', stdout),_print9(e10, e11, e12, e13, e14, e15, e16, e17, e18)
#define _print19(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19) _print10(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10),fputc(' ', stdout),_print9(e11, e12, e13, e14, e15, e16, e17, e18, e19)
#define _print20(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20) _print10(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10),fputc(' ', stdout),_print10(e11, e12, e13, e14, e15, e16, e17, e18, e19, e20)
#define _GET_PRINT_MACRO_NAME(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, NAME, ...) NAME
#define print(...) _GET_PRINT_MACRO_NAME(__VA_ARGS__, _print20, _print19, _print18, _print17, _print16, _print15, _print14, _print13, _print12, _print11, _print10, _print9, _print8, _print7, _print6, _print5, _print4, _print3, _print2, _print1)(__VA_ARGS__),fputc('\n', stdout)
#define print0(...) _GET_PRINT_MACRO_NAME(__VA_ARGS__, _print20, _print19, _print18, _print17, _print16, _print15, _print14, _print13, _print12, _print11, _print10, _print9, _print8, _print7, _print6, _print5, _print4, _print3, _print2, _print1)(__VA_ARGS__)
#define printF(...) _GET_PRINT_MACRO_NAME(__VA_ARGS__, _print20, _print19, _print18, _print17, _print16, _print15, _print14, _print13, _print12, _print11, _print10, _print9, _print8, _print7, _print6, _print5, _print4, _print3, _print2, _print1)(__VA_ARGS__),fputc('\n', stdout),fflush(stdout)
#define print0F(...) _GET_PRINT_MACRO_NAME(__VA_ARGS__, _print20, _print19, _print18, _print17, _print16, _print15, _print14, _print13, _print12, _print11, _print10, _print9, _print8, _print7, _print6, _print5, _print4, _print3, _print2, _print1)(__VA_ARGS__),fflush(stdout)
