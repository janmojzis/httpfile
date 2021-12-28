#include "percent.h"

static unsigned char fromhex(unsigned char t) {
    unsigned char u;

    u = t - 'a';
    if (u < 6) return u + 10;
    u = t - 'A';
    if (u < 6) return u + 10;
    u = t - '0';
    if (u < 10) return u;
    return 0;
}

void percent(stralloc *sa) {
    long long i;
    long long j;
    char ch;

    i = j = 0;
    for (;;) {
        if (j >= sa->len) break;
        ch = sa->s[j++];
        if (ch == '%') {
            if (j >= sa->len) break;
            ch = 16 * fromhex(sa->s[j++]);
            if (j >= sa->len) break;
            ch += fromhex(sa->s[j++]);
        }
        sa->s[i++] = ch;
    }
    sa->len = i;
}
