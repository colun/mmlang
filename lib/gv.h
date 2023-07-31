struct gvRGB {
    int r;
    int g;
    int b;
    gvRGB() : r(0), g(0), b(0) {}
    gvRGB(int r, int g, int b) : r(r), g(g), b(b) {
    }
    gvRGB(const std::vector<int> & rgb) : r(rgb[0]), g(rgb[1]), b(rgb[2]) {
    }
    int toInt() const {
        return ((r & 255) << 16) | ((g & 255) << 8) | (b & 255);
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
FILE * gv$file = NULL;
bool gv$flushFlag = true;
void gvInit() {
    if(gv$file==NULL) {
        gv$file = fopen("result.gv", "w+");
    }
}
void gvFlush() {
    gv$flushFlag = false;
    if(gv$file) {
        fflush(gv$file);
    }
}
void gvFlush_() {
    if(gv$flushFlag) {
        assert(gv$file);
        fflush(gv$file);
    }
}
inline bool gvTime() {
    gvInit();
    fprintf(gv$file, "n\n");
    gvFlush_();
    return true;
}
inline bool gvTime(double t) {
    gvInit();
    fprintf(gv$file, "n %g\n", t);
    gvFlush_();
    return true;
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
void gvCircle(double x, double y) {
    gvInit();
    fprintf(gv$file, "c %g %g\n", x, y);
    gvFlush_();
}
void gvCircle(double x, double y, double r, gvRGB rgb) {
    gvInit();
    fprintf(gv$file, "c %g %g %d %g\n", x, y, rgb.toInt(), r);
    gvFlush_();
}
void gvCircle(double x, double y, double r) {
    gvCircle(x, y, r, gvRGB(0, 0, 0));
}
void gvCircle(double x, double y, gvRGB rgb) {
    gvInit();
    fprintf(gv$file, "c %g %g %d\n", x, y, rgb.toInt());
    gvFlush_();
}
void gvRect(double x, double y, double x2, double y2, double r, gvRGB rgb) {
    double bx = min(x, x2) - fabs(r);
    double by = min(y, y2) - fabs(r);
    double ex = max(x, x2) + fabs(r);
    double ey = max(y, y2) + fabs(r);
    int c = rgb.toInt();
    gvInit();
    fprintf(gv$file, "p %d", rgb.toInt());
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
    fprintf(gv$file, "t %g %g %d %g ", x, y, rgb.toInt(), r);
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
    fprintf(gv$file, "t %g %g %d 0.5 ", x, y, rgb.toInt());
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
    fprintf(gv$file, "tl %g %g %d %g ", x, y, rgb.toInt(), r);
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
    fprintf(gv$file, "tl %g %g %d 0.5 ", x, y, rgb.toInt());
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
void gvLine(double x1, double y1, double x2, double y2, double r, gvRGB rgb) {
    double odx = x2-x1;
    double ody = y2-y1;
    if(fabs(odx)<=1e-9 && fabs(ody)<=1e-9) {
        return;
    }
    double rate = r / sqrt(odx*odx+ody*ody);
    double dx = odx * rate;
    double dy = ody * rate;
    gvInit();
    fprintf(gv$file, "p %d", rgb.toInt());
    fprintf(gv$file, " %g %g", x2-dy*(0.05/(1+sqrt(2))), y2+dx*(0.05/(1+sqrt(2))));
    fprintf(gv$file, " %g %g", x2-dx*(0.05*sqrt(2)/(1+sqrt(2)))-dy*0.05, y2-dy*(0.05*sqrt(2)/(1+sqrt(2)))+dx*0.05);
    fprintf(gv$file, " %g %g", x1+dx*(0.05*sqrt(2)/(1+sqrt(2)))-dy*0.05, y1+dy*(0.05*sqrt(2)/(1+sqrt(2)))+dx*0.05);
    fprintf(gv$file, " %g %g", x1-dy*(0.05/(1+sqrt(2))), y1+dx*(0.05/(1+sqrt(2))));
    fprintf(gv$file, " %g %g", x1+dy*(0.05/(1+sqrt(2))), y1-dx*(0.05/(1+sqrt(2))));
    fprintf(gv$file, " %g %g", x1+dx*(0.05*sqrt(2)/(1+sqrt(2)))+dy*0.05, y1+dy*(0.05*sqrt(2)/(1+sqrt(2)))-dx*0.05);
    fprintf(gv$file, " %g %g", x2-dx*(0.05*sqrt(2)/(1+sqrt(2)))+dy*0.05, y2-dy*(0.05*sqrt(2)/(1+sqrt(2)))-dx*0.05);
    fprintf(gv$file, " %g %g", x2+dy*(0.05/(1+sqrt(2))), y2-dx*(0.05/(1+sqrt(2))));
    fprintf(gv$file, "\n");
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
    double odx = x2-x1;
    double ody = y2-y1;
    if(fabs(odx)<=1e-9 && fabs(ody)<=1e-9) {
        return;
    }
    double rate = r / sqrt(odx*odx+ody*ody);
    double dx = odx * rate;
    double dy = ody * rate;
    double x2_base = x2+dx*0.1;
    double y2_base = y2+dy*0.1;
    double dx0 = dx*0.1*tan(M_PI*15/180);
    double dy0 = dy*0.1*tan(M_PI*15/180);
    double x2_3 = x2_base-dx*(0.1/sin(M_PI*15/180));
    double y2_3 = y2_base-dy*(0.1/sin(M_PI*15/180));
    double x2_4 = x2_3-dx*(0.05/tan(M_PI*15/180));
    double y2_4 = y2_3-dy*(0.05/tan(M_PI*15/180));
    double x2_5 = x2_base-dx*(1.0*cos(M_PI*15/180));
    double y2_5 = y2_base-dy*(1.0*cos(M_PI*15/180));
    double x2_6 = x2_5-dx*(0.1*sin(M_PI*15/180));
    double y2_6 = y2_5-dy*(0.1*sin(M_PI*15/180));
    double dx5 = dx*(1.0*sin(M_PI*15/180));
    double dy5 = dy*(1.0*sin(M_PI*15/180));
    double dx6 = dx5-dx*(0.1*cos(M_PI*15/180));
    double dy6 = dy5-dy*(0.1*cos(M_PI*15/180));
    gvInit();
    fprintf(gv$file, "p %d", rgb.toInt());
    fprintf(gv$file, " %g %g", x2-dy0, y2+dx0);
    fprintf(gv$file, " %g %g", x2_5-dy5, y2_5+dx5);
    fprintf(gv$file, " %g %g", x2_6-dy6, y2_6+dx6);
    fprintf(gv$file, " %g %g", x2_4-dy*0.05, y2_4+dx*0.05);
    fprintf(gv$file, " %g %g", x1+dx*(0.05*sqrt(2)/(1+sqrt(2)))-dy*0.05, y1+dy*(0.05*sqrt(2)/(1+sqrt(2)))+dx*0.05);
    fprintf(gv$file, " %g %g", x1-dy*(0.05/(1+sqrt(2))), y1+dx*(0.05/(1+sqrt(2))));
    fprintf(gv$file, " %g %g", x1+dy*(0.05/(1+sqrt(2))), y1-dx*(0.05/(1+sqrt(2))));
    fprintf(gv$file, " %g %g", x1+dx*(0.05*sqrt(2)/(1+sqrt(2)))+dy*0.05, y1+dy*(0.05*sqrt(2)/(1+sqrt(2)))-dx*0.05);
    fprintf(gv$file, " %g %g", x2_4+dy*0.05, y2_4-dx*0.05);
    fprintf(gv$file, " %g %g", x2_6+dy6, y2_6-dx6);
    fprintf(gv$file, " %g %g", x2_5+dy5, y2_5-dx5);
    fprintf(gv$file, " %g %g", x2+dy0, y2-dx0);
    fprintf(gv$file, "\n");
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
#else
#define gvFlush(...)
#define gvFps(...) false
#define gvTime(...) false
#define gvCircle(...)
#define gvRect(...)
#define gvText(...)
#define gvTextLeft(...)
#define gvLine(...)
#define gvArrow(...)
#endif
