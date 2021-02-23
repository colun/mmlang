#define unlikely(expr) (__builtin_expect(!!(expr), 0))
