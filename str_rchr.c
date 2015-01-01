/* Public domain. */

#include "str.h"

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

