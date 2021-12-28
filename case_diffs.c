/* Public domain. */

#include "case.h"

long long case_diffs(register const char *s, register const char *t) {
    register unsigned char x;
    register unsigned char y;

    for (;;) {
        x = *s++ - 'A';
        if (x <= 'Z' - 'A')
            x += 'a';
        else
            x += 'A';
        y = *t++ - 'A';
        if (y <= 'Z' - 'A')
            y += 'a';
        else
            y += 'A';
        if (x != y) break;
        if (!x) break;
    }
    return ((long long) (unsigned long long) x) -
           ((long long) (unsigned long long) y);
}
