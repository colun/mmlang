#define likely(expr) (__builtin_expect(!!(expr), 1))
