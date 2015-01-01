#include "fmt.h"

long long fmt_longlong(char *s, long long u) {

    if (u >= 0) return fmt_ulonglong(s, u);
    if (s) *s++='-';
    return fmt_ulonglong(s, -u) + 1;
}

long long fmt_longlong0(char *s, long long u, long long n) {

    if (u >= 0) return fmt_ulonglong0(s, u, n);
    if (s) *s++='-';
    return fmt_ulonglong0(s, -u, n) + 1;
}
