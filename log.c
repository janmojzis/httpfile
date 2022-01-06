/*
20211217
Jan Mojzis
Public domain.

The 'log' library is used to write log messages
to standard error output including source file,
function and line number.
Non-printable characters are escaped.

Log format:
time: name: level: ip: message (error){file:line}[id]
time .......... optional
ip ............ optional
name .......... optional
{file:line} ... in debug mode
[id] .......... optional
*/

#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "e.h"
#include "randommod.h"
#include "log.h"

static const char *logname = 0;
static const char *logipstr = 0;
static const char *logid = "";
static char logidbuf[9];
static int loglevel = 1;
static int logtime = 0;
static long long loglimit = 200;

void log_level(int level) {
    loglevel = level;
    if (level < 0) loglevel = 0;
    if (level > 4) loglevel = 4;
}

void log_name(const char *name) { logname = name; }
void log_time(int flag) { logtime = flag; }
void log_limit(long long limit) { loglimit = limit; }
void log_ip(const char *ip) { logipstr = ip; }

static char buf[256];
static unsigned long long buflen = 0;

static void flush(void) {

    char *b = buf;
    long long r;

    while (buflen > 0) {
        r = write(2, b, buflen);
        if (r < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN) continue;
            if (errno == EWOULDBLOCK) continue;
            break;
        }
        if (r == 0) break;
        b += r;
        buflen -= r;
    }
    buflen = 0;
}

static void outch(const char x) {
    if (buflen >= sizeof buf) flush();
    buf[buflen++] = x;
}

static void outsescape(const char *x, int flaglf, long long *counter) {

    long long i;

    for (i = 0; x[i]; ++i) {
        if (counter && ++*counter > loglimit) {
            outch('.');
            outch('.');
            outch('.');
            break;
        }
        if (x[i] == '\n') {
            if (flaglf) { outch('\n'); }
            else {
                outch('\\');
                outch('n');
            }
        }
        else if (x[i] == '\r') {
            outch('\\');
            outch('r');
        }
        else if (x[i] == '\t') {
            outch('\\');
            outch('t');
        }
        else if (x[i] < 32 || x[i] > 126) {
            outch('\\');
            outch('x');
            outch("0123456789abcdef"[(x[i] >> 4) & 15]);
            outch("0123456789abcdef"[(x[i] >> 0) & 15]);
        }
        else {
            outch(x[i]);
        }
    }
}
#define outs(x) outsescape((x), 1, 0);

static char *numtostr(char *strbuf, long long strbuflen, long long n,
                      long long cnt) {

    long long len = 0;
    unsigned long long n1, n2;
    int flagsign = 0;

    if (cnt > strbuflen - 1) cnt = strbuflen - 1;

    n1 = n2 = (unsigned long long) n;
    if (n < 0) {
        n1 = -n1;
        n2 = -n2;
        flagsign = 1;
    }

    do {
        n1 /= 10;
        ++len;
    } while (n1);
    if (flagsign) ++len;
    strbuf += len;
    if (cnt > len) strbuf += cnt - len;
    *strbuf = 0;

    do {
        *--strbuf = '0' + (n2 % 10);
        n2 /= 10;
    } while (n2);
    while (cnt > len) {
        *--strbuf = '0';
        --cnt;
    }
    if (flagsign) *--strbuf = '-';

    return strbuf;
}

#define STATICBUFSIZE 41

static void outnum(unsigned long long n, unsigned long long cnt) {

    char numbuf[STATICBUFSIZE];
    outs(numtostr(numbuf, sizeof numbuf, n, cnt));
}

void log_9_(int level, int flagerror, const char *f, unsigned long long l,
            const char *s0, const char *s1, const char *s2, const char *s3,
            const char *s4, const char *s5, const char *s6, const char *s7,
            const char *s8) {
    const char *s[9];
    long long i;
    const char *m;
    long long counter = 0;
    long long *counterptr = &counter;

    if (level > loglevel) return;

    if (loglevel <= 2) counterptr = 0;

    s[0] = s0;
    s[1] = s1;
    s[2] = s2;
    s[3] = s3;
    s[4] = s4;
    s[5] = s5;
    s[6] = s6;
    s[7] = s7;
    s[8] = s8;

    switch (level) {
        case 1:
            m = "fatal";
            break;
        case 2:
            if (flagerror == 1)
                m = "error";
            else if (flagerror == 2)
                m = "warning";
            else
                m = "info";
            break;
        case 3:
            m = "debug";
            break;
        case 4:
            m = "tracing";
            break;
        default:
            m = "unknown";
            break;
    }

    /* time: name: level: ip: message (error){file:line}[id] */

    /* 'time:' */
    do {
        struct tm *t;
        int saved_errno = errno;
        time_t secs = time(0);
        if (!level) break;   /* don't print in usage level */
        if (!logtime) break; /* don't print when logtime = 0 */

        t = localtime(&secs);
        outnum(t->tm_year + 1900, 4);
        outs("-");
        outnum(t->tm_mon + 1, 2);
        outs("-");
        outnum(t->tm_mday, 2);
        outs(" ");
        outnum(t->tm_hour, 2);
        outs(":");
        outnum(t->tm_min, 2);
        outs(":");
        outnum(t->tm_sec, 2);
        outs(": ");
        errno = saved_errno;
    } while (0);

    /* 'name:' */
    do {
        if (!level) break;   /* don't print in usage level */
        if (!logname) break; /* don't print when logname = 0 */
        outsescape(logname, 0, counterptr);
        outs(": ");
    } while (0);

    /* 'level:' */
    do {
        if (!level) break; /* don't print in usage level */
        outs(m);
        outs(": ");
    } while (0);

    /* 'ip:' */
    do {
        if (!level) break;    /* don't print in usage level */
        if (!logipstr) break; /* don't print when logipstr = 0 */
        outsescape(logipstr, 0, counterptr);
        outs(": ");
    } while (0);

    /* 'message' */
    for (i = 0; i < 9 && s[i]; ++i) outsescape(s[i], !level, counterptr);
    outs(" ");

    /* '(error)' */
    do {
        if (!level) break;     /* don't print in usage level   */
        if (!errno) break;     /* don't print when errno = 0   */
        if (!flagerror) break; /* don't print when disabled    */
        if (level >= 3) break; /* don't print in debug message */
        outs("(");
        outs(e_str(errno));
        outs(")");
    } while (0);

    /* {file:line} */
    do {
        if (!f) break;            /* don't print when no f                  */
        if (!l) break;            /* don't print when no l                  */
        if (!level) break;        /* don't print in usage level             */
        if (loglevel <= 2) break; /* print only when debug verbosity is set */
        outs("{");
        outs(f);
        outs(":");
        outnum(l, 0);
        outs("}");
    } while (0);

    /* [id] */
    do {
        if (loglevel <= 1) break; /* don't print in usage, fatal level */
        if (!logid) break;        /* don't print when logid = 0 */
        if (logid[0] == 0) break; /* don't print when logid = "" */
        outs("[");
        outsescape(logid, 0, counterptr);
        outs("]");
    } while (0);

    outs("\n");
    flush();
    return;
}

static char staticbuf[9][STATICBUFSIZE];
static int staticbufcounter = 0;

char *logip(unsigned char *ip) {
    staticbufcounter = (staticbufcounter + 1) % 9;
    if (memcmp(ip, "\0\0\0\0\0\0\0\0\0\0\377\377", 12)) {
        struct sockaddr_in6 sa;
        memcpy(&(sa.sin6_addr), ip, 16);
        inet_ntop(AF_INET6, &(sa.sin6_addr), staticbuf[staticbufcounter],
                  STATICBUFSIZE);
    }
    else {
        struct sockaddr_in sa;
        memcpy(&(sa.sin_addr), ip + 12, 4);
        inet_ntop(AF_INET, &(sa.sin_addr), staticbuf[staticbufcounter],
                  STATICBUFSIZE);
    }
    return staticbuf[staticbufcounter];
}

char *logport(unsigned char *port) {
    staticbufcounter = (staticbufcounter + 1) % 9;
    return numtostr(staticbuf[staticbufcounter], STATICBUFSIZE,
                    port[0] << 8 | port[1], 0);
}

char *lognum(long long num) {
    staticbufcounter = (staticbufcounter + 1) % 9;
    return numtostr(staticbuf[staticbufcounter], STATICBUFSIZE, num, 0);
}
char *lognum0(long long num, long long cnt) {
    staticbufcounter = (staticbufcounter + 1) % 9;
    return numtostr(staticbuf[staticbufcounter], STATICBUFSIZE, num, cnt);
}

static void tohex(char *x, long long xlen, unsigned char *y, long long ylen) {
    long long i;
    for (i = 0; i < ylen; ++i) {
        if (i == (xlen - 3) / 2) {
            x[2 * i] = '.';
            x[2 * i + 1] = '.';
            x[2 * i + 2] = 0;
            return;
        }
        x[2 * i] = "0123456789abcdef"[(y[i] >> 4) & 15];
        x[2 * i + 1] = "0123456789abcdef"[(y[i] >> 0) & 15];
    }
    x[2 * i] = 0;
}

char *loghex(unsigned char *y, long long ylen) {
    char *x;
    staticbufcounter = (staticbufcounter + 1) % 9;
    x = staticbuf[staticbufcounter];
    tohex(x, STATICBUFSIZE, y, ylen);
    return x;
}

void log_id(const char *id) {

    if (!id) id = getenv("LOG_ID");
    if (!id) {
        static char chars[] =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRTSUVWXYZ0123456789";
        unsigned long long i;
        for (i = 0; i < sizeof logidbuf; ++i) {
            logidbuf[i] = chars[randommod(sizeof chars - 1)];
        }
        logidbuf[sizeof logidbuf - 1] = 0;
        id = logidbuf;
    }
    logid = id;
    (void) setenv("LOG_ID", id, 1);
}
