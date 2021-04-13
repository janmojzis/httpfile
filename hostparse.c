#include "stralloc.h"
#include "hostparse.h"

void hostparse(stralloc *host) {

	long long i, j, colonpos, coloncount = 0;
    char ch;

    /* IPv6 in brackets */
    if (host->len > 0 && host->s[0] == '[') {
        j = 0;
        for (i = 1; i < host->len; ++i) {
            ch = host->s[i];
            if (ch == ']') break;
            host->s[j++] = ch;
        }
        host->len = j;
        return;
    }

    for (i = 0; i < host->len; ++i) {
        if (host->s[i] == ':') {
            colonpos = i;
            ++coloncount;
        }
    }
    if (coloncount == 1) {
        host->len = colonpos;
    }
}
