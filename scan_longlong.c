#include "scan.h"

long long scan_longlong(register const char *s, register long long *i) {

    int sign; unsigned long long u; register long long len;
    len = scan_plusminus(s,&sign); s += len;
    len += scan_ulonglong(s,&u);
    if (sign < 0) *i = -u; else *i = u;
    return len;
}

