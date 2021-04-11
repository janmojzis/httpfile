#include "rangeparser.h"

int rangeparser(long long *first, long long *last, char *x, long long xlen, long long length) {

    long long pos, i, min = -1, max = -1;
    unsigned long long c;
    
    for (pos = 0; pos < xlen; ++pos) {
        if (x[pos] == '-') break;
    }
    /* '-' not found */
    if (pos == xlen) return 0;

    /* parse min */
    if (pos > 0) {
        min = 0;
        for (i = 0; i < pos; ++i) {
            c = (unsigned long long) (unsigned char) (x[i] - '0');
            if (c > 10) return 0;
            min = min * 10 + c;
        }
    }

    /* parse max */
    if (pos + 1 < xlen) {
        max = 0;
        for (i = pos + 1; i < xlen; ++i) {
            c = (unsigned long long) (unsigned char) (x[i] - '0');
            if (c > 10) return 0;
            max = max * 10 + c;
        }
    }

    /* no min, no max */
    if (min < 0 && max < 0) return 0;

    /* min > max */
    if (max >= 0 && min > max) return 0;

    /* '2-' -> 2-47021/47022 */
    if (max < 0) {
        *first = min;
        *last = length - 1;
    }

    /* '-2' -> 47020-47021/47022 */
    if (min < 0) {
        *last = length - 1;
        *first = length - max;
        if (*first < 0) *first = 0;
    }

    /* '1-1' ->  1-1/47022 */
    if (min >= 0 && max >= 0) {
        *first = min;
        *last = max;
        if (*last > length - 1) *last = length - 1;
    }

    if (*first < 0) return 0;
    if (*last < 0) return 0;
    if (*first >= length) return 0;
    if (*first > *last) return 0;

    return 1;
}
