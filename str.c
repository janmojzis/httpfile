#include "str.h"

long long str_len(const char *str) {

    const char *s;

    for (s = str; *s; ++s);
    return (s - str);
}

int str_equal(const char *y, const char *x) {

    long long i;

    for (i = 0; x[i] && y[i]; ++i) if (y[i] != x[i]) return 0;
    return (y[i] == x[i]);
}

long long str_chr(const char *str, int c) {

    const char *s;
    char ch = c;

    for (s = str; (*s && *s != ch); ++s);
    return (s - str);
}

long long str_rchr(const char *str, int c) {

    const char *s;
    const char *u = 0;
    char ch = c;

    for (s = str; *s; ++s) {
        if (*s == ch) u = s;
    }
    if (!u) u = s;
    return (u - str);
}
