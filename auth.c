#include "auth.h"
#include "pathdecode.h"
#include "percent.h"
#include "e.h"
#include "str.h"
#include "cdb.h"
#include "open.h"
#include "byte.h"

#ifdef AUTH

static int fd = -1;
static struct cdb c;

static stralloc tmp = {0};
static stralloc data = {0};

void auth_init(const char *fn) {

    if (!fn) return;
    fd = open_read(fn);
    if (fd == -1) return;
    cdb_init(&c,fd);
    return;
}

int auth_basic(stralloc *host, stralloc *path, stralloc *header) {

    long long x;
    char *key;
    long long len;
    int r;

    header->s[header->len] = 0;

    if (fd == -1) return 0;

    if (!stralloc_copys(&tmp,"/")) return -1;
    if (!stralloc_cat(&tmp,host)) return -1;
    if (!stralloc_cats(&tmp,"/")) return -1;
    if (!stralloc_cat(&tmp,path)) return -1;
    pathdecode(&tmp);
    if (!stralloc_0(&tmp)) return -1;

    for(;;) {
        x = str_rchr(tmp.s, '/');
        if (!tmp.s[x]) break;
        tmp.s[x] = 0;
        if (x > 0) {
            len = x - 1;
            key = tmp.s + 1;
        }
        else {
            key = tmp.s;
            len = x;
        }
        r = cdb_find(&c,key,len);
        if (r == -1) return -1;
        if (r) goto A;
    }
    return 0;

A:
    for(;;) {
        if (cdb_datalen(&c) == header->len) {
            if (!stralloc_ready(&data, cdb_datalen(&c))) return -1;
            if (cdb_read(&c,data.s,cdb_datalen(&c),cdb_datapos(&c)) == -1) return -1;
            if (byte_isequal(header->s, header->len, data.s)) return 0;
        }
        r = cdb_findnext(&c,key,len);
        if (r == -1) return -1;
        if (!r) return 1;
    }
    return 1;

}

#endif
