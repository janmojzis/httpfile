/* Public domain. */

#include "str.h"

long long str_chr(const char *str, int c) {

    const char *s;
    char ch = c;

    for (s = str; (*s && *s != ch); ++s);
    return (s - str);
}

