#include "pathdecode.h"

void pathdecode(stralloc *fn) {
    long long i;
    long long j;
    char ch;

    j = 0;
    for (i = 0; i < fn->len; ++i) {
        ch = fn->s[i];
        if (ch == 0) ch = '\n';
        if (j && (fn->s[j - 1] == '/')) {
            if (ch == '.') ch = ':';
            if (ch == '/') continue;
        }
        fn->s[j++] = ch;
    }
    fn->len = j;
}
