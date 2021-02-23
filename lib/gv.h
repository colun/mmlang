struct gvRGB {
    int r;
    int g;
    int b;
    gvRGB(int r, int g, int b) : r(r), g(g), b(b) {
    }
    int toInt() const {
        return ((r & 255) << 16) | ((g & 255) << 8) | (b & 255);
    }
};
#ifdef GV_ENABLE
FILE * gv$file = NULL;
void gvInit() {
    if(gv$file==NULL) {
        gv$file = fopen("result.gv", "w+");
    }
}
void gvFlush() {
    assert(gv$file);
    fflush(gv$file);
}
inline void gvTime() {
    gvInit();
    fprintf(gv$file, "n\n");
    gvFlush();
}
inline void gvTime(double t) {
    gvInit();
    fprintf(gv$file, "n %g\n", t);
    gvFlush();
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
void gvText(double x, double y, double r, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "t %g %g %d %g ", x, y, rgb.toInt(), r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush();
}
void gvText(double x, double y, double r, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "t %g %g 0 %g ", x, y, r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush();
}
void gvText(double x, double y, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "t %g %g %d 0.5 ", x, y, rgb.toInt());
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush();
}
void gvText(double x, double y, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "t %g %g 0 0.5 ", x, y);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush();
}
void gvTextLeft(double x, double y, double r, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tl %g %g %d %g ", x, y, rgb.toInt(), r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush();
}
void gvTextLeft(double x, double y, double r, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tl %g %g 0 %g ", x, y, r);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush();
}
void gvTextLeft(double x, double y, gvRGB rgb, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tl %g %g %d 0.5 ", x, y, rgb.toInt());
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush();
}
void gvTextLeft(double x, double y, const char * format = "?", ...) {
    gvInit();
    fprintf(gv$file, "tl %g %g 0 0.5 ", x, y);
    va_list arg;
    va_start(arg, format);
    vfprintf(gv$file, format, arg);
    va_end(arg);
    fprintf(gv$file, "\n");
    gvFlush();
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
    gvFlush();
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
inline bool gvFps(double t, double fps=60.0) {
    return false;
}
#define gvTime(...)
#define gvText(...)
#define gvArrow(...)
#endif
