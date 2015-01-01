#include <unistd.h>
#include <stdio.h>
#include "strerr.h"
#include "stralloc.h"
#include "getln.h"
#include "buffer.h"
#include "fmt.h"
#include "byte.h"
#include "cdb_make.h"
#include "pathdecode.h"
#include "percent.h"
#include "open.h"
#include "base64.h"
#include "case.h"

#define FATAL "httpfile-access: fatal: "

char *fntemp;
char *fn;

stralloc line = {0};
int match = 1;

stralloc path = {0};
stralloc key = {0};
stralloc val = {0};

struct cdb_make c;

void die_nomem(void){strerr_die2x(111,FATAL,"out of memory");}
void usage(void){strerr_die1x(100,"httpfile-access: usage: httpfile-access access.cdb access.tmp < access.txt");}
void die_write(void){strerr_die4sys(111,FATAL,"unable to write to ",fntemp,": ");}
void die_rename(const char *f1, const char *f2){strerr_die6sys(111,FATAL,"unable to move ",f1," to ",f2,": ");}
void die_read(void){strerr_die2sys(111,FATAL,"unable to read input: ");}
void die_create(const char *f){strerr_die4sys(111,FATAL,"unable to create ",f,": ");}



int main(int argc,char **argv) {

    long long colon;
    char *x;
    long long len;
    int fd;
    char ch;

    fn = argv[1];
    if (!fn) usage();
    fntemp = argv[2];
    if (!fntemp) usage();

    fd = open_trunc(fntemp);
    if (fd == -1) die_create(fntemp);
    if (cdb_make_start(&c,fd) == -1) die_write();

    while (match) {
        if (getln(buffer_0,&line,&match,'\n') == -1)
            die_read();

        x = line.s; len = line.len;

        if (len <= 0) break;
        if (x[0] == '#') continue;
        if (x[0] == '\n') continue;

        while (len > 0) {
            ch = x[len - 1];
            if (ch != '\n') if (ch != ' ') if (ch != '\t') break;
            --len;
        }
        line.len = len;

        if (!stralloc_copys(&key,"/")) die_nomem();

        /* host */
        colon = byte_chr(x,len,':');
        if (colon == len) continue;
        case_lowerb(x,colon);
        if (!stralloc_catb(&key,x,colon)) die_nomem();
        if (!stralloc_cats(&key,"/")) die_nomem();
        x += colon + 1; len -= colon + 1;

        /* path */
        colon = byte_chr(x,len,':');
        if (colon == len) continue;
        if (!stralloc_copyb(&path,x,colon)) die_nomem();
        percent(&path);
        if (!stralloc_cat(&key,&path)) die_nomem();
        pathdecode(&key);
        x += colon + 1; len -= colon + 1;

        if (key.len > 0) {
            if (key.s[key.len - 1] == '/') --key.len;
        }

        /* user:password */
        if (base64_encode(&val,(unsigned char *)x,len) == -1) die_nomem();
        if (cdb_make_add(&c,key.s + 1,key.len - 1,val.s,val.len) == -1) die_write();
    }

    if (cdb_make_finish(&c) == -1) die_write();
    if (fsync(fd) == -1) die_write();
    if (close(fd) == -1) die_write(); /* NFS stupidity */
    if (rename(fntemp,fn)) die_rename(fntemp, fn);
    _exit(0);
}
