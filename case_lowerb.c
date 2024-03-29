/* Public domain. */

#include "case.h"

void case_lowerb(char *s, long long len) {
    unsigned char x;
    while (len > 0) {
        --len;
        x = *s - 'A';
        if (x <= 'Z' - 'A') *s = x + 'a';
        ++s;
    }
}
