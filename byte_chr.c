#include "byte.h"

long long byte_chr(const void *yv,long long ylen, int c) {

    const unsigned char *y = yv;
    char ch = c;

    if (ylen <= 0) return ylen;

    while (ylen > 0) {
        if (*y == ch) break;
        ++y; --ylen;
    }
    return y - (const unsigned char *)yv;
}
