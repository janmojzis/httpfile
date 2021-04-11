#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "stralloc.h"
#include "pathdecode.h"
#include "httpdate.h"
#include "seconds.h"
#include "percent.h"
#include "case.h"
#include "log.h"
#include "byte.h"
#include "str.h"
#include "numtostr.h"
#include "filetype.h"
#include "file.h"
#include "droproot.h"
#include "alloc.h"
#include "e.h"
#include "timeoutwrite.h"
#include "rangeparser.h"

#define WRITETIMEOUT 5
#define REQUESTTIMEOUT 30

static int flagverbose = 1;

static void _die(int x) {
    alloc_freeall();
    _exit(x);
}

static void die_nomem(void)  { log_f1("out of memory"); _die(111); }
static void die_droproot(void)  { log_f1("unable to drop privileges"); _die(111); }

static stralloc line = {0};
static stralloc protocol = {0};
static int protocolnum = 0;
static stralloc method = {0};
static stralloc url = {0};
static stralloc host = {0};
static stralloc path = {0};
static stralloc ims = {0};
static stralloc mtimestr = {0};
static long long mtime = 0;
static stralloc field = {0};
static int flagbody = 1;
static stralloc redirurl = {0};
static stralloc nowstr = {0};
static stralloc fn = {0};
static stralloc contenttype = {0};
static char *filecontent;
static long long filelength = 0;
static stralloc range = {0};
static long long rangefirst = -1;
static long long rangelast = -1;
static int fd;
static stralloc auth = {0};
#define response line

static char *customheaders[16];
static long long customheaderslen = 0;

static void writeall(const char *x, long long xlen) {

    long long w;

    while (xlen > 0) {
        w = xlen; if (w > 1024) w = 1024;
        w = timeoutwrite(WRITETIMEOUT, 1, x, w);
        if (w <= 0) _die(111);
        x += w;
        xlen -= w;
    }
}

static void out_flush(void) {
    writeall(response.s, response.len);
    if (!stralloc_copys(&response, "")) die_nomem();
}

static void out_puts(const char *x) {
    if (!stralloc_cats(&response, x)) die_nomem();
}

static void out_put(const char *x, long long xlen) {
    if (!stralloc_catb(&response, x, xlen)) die_nomem();
}

static void out_body(const char *x, long long xlen) {
#if XXXTODO
    if (!stralloc_0(&response)) die_nomem();
    --response.len;
    log_d2("response headers: ", response.s);
#endif
    out_flush();
    if (flagbody) {
        writeall(x, xlen);
    }
}

static void customheaders_add(char *x) {
    if ((unsigned long long) customheaderslen <= sizeof customheaders / sizeof customheaders[0]) {
        customheaders[customheaderslen++] = x;
    }
}

static void customheaders_put(void) {

    long long i, j;

    for (i = 0; i < customheaderslen; ++i) {
        j = str_chr(customheaders[i], '\n');
        customheaders[i][j] = 0;
        j = str_chr(customheaders[i], ':');
        if (str_len(customheaders[i]) > 0 && j != str_len(customheaders[i])) {
            out_puts(customheaders[i]);
            out_puts("\r\n");
        }
    }
}

static void header(const char *code, const char *message) {

    if (protocolnum == 1)
        out_puts("HTTP/1.0 ");
    else
        out_puts("HTTP/1.1 ");
    out_puts(code);
    out_puts(message);
    out_puts("\r\nServer: httpfile\r\nAccept-Ranges: bytes\r\nDate:");
    if (!httpdate(&nowstr, seconds())) die_nomem();
    out_put(nowstr.s, nowstr.len);
    out_puts("\r\n");
    customheaders_put();
    if (!stralloc_0(&url)) die_nomem();
    if (!stralloc_0(&host)) die_nomem();
    log_i5(host.s, url.s, " ", code, message);
}

void drop(int x) {
    (void) x;
    if (protocolnum >= 2) {
        shutdown(1, 1);
        sleep(1);
    }
    _die(x);
}

static void barf(const char *code, const char *message) {

  if (protocolnum > 0) {
    header(code, message);
    if (str_equal("401 ", code)) {
      out_puts("WWW-Authenticate: Basic realm=\"authorization required\"\r\n");
    }
    out_puts("Content-Length: ");
    out_puts(numtostr(0, str_len(message) + 28));
    out_puts("\r\n");
    if (protocolnum == 2)
      out_puts("Connection: close\r\n");
    out_puts("Content-Type: text/html\r\n\r\n");
  }
  if (flagbody) {
    out_puts("<html><body>");
    out_puts(message);
    out_puts("</body></html>\r\n");
  }
  out_flush();
  drop(0);
}


static void redirect(char *prefix, char *suffix) {

    if (!stralloc_copys(&redirurl, prefix)) die_nomem();
    if (!stralloc_cats(&redirurl, "//")) die_nomem();
    if (!stralloc_cat(&redirurl, &host)) die_nomem();
    if (path.len <= 0 || path.s[0] != '/') {
        if (!stralloc_cats(&redirurl, "/")) die_nomem();
    }
    if (!stralloc_cat(&redirurl, &path)) die_nomem();
    if (!stralloc_cats(&redirurl, suffix)) die_nomem();
    if (protocolnum > 0) {
        header("301 ", "moved permanently");
        out_puts("Content-Length: 0\r\nLocation: ");
        out_put(redirurl.s, redirurl.len);
        out_puts("\r\n\r\n");
    }
    out_flush();
    if (protocolnum < 2) _die(0);
}

static void redirecthttps(void) {

    redirect("https:", "");
}

static void get(void) {

    struct stat st;

    if (path.len <= 0 || (path.s[path.len - 1] == '/'))
        if (!stralloc_cats(&path, "index.html")) die_nomem();
    percent(&path);
    if (!stralloc_copys(&fn,"./")) die_nomem();
    if (auth.len) {
        if (!stralloc_cats(&fn, "@")) die_nomem();
        if (!stralloc_cat(&fn, &auth)) die_nomem();
        if (!stralloc_cats(&fn, "/")) die_nomem();
        /* check if authorization string is valid */
        if (!stralloc_0(&fn)) die_nomem();
        --fn.len;
        if (stat(fn.s, &st) != 0) barf("401 ", "unauthorized");
    }
    if (!stralloc_cat(&fn, &host)) die_nomem();
    if (!stralloc_cats(&fn,"/")) die_nomem();
    if (!stralloc_cat(&fn, &path)) die_nomem();
    pathdecode(&fn);
    if (!stralloc_0(&fn)) die_nomem();

    errno = 0;
    fd = file_open(fn.s, &filecontent, &filelength, &mtime);
    log_d2("opening file: ", fn.s);
    if (fd == -1) {
        switch (errno) {
            case EISDIR:
                redirect("", "/"); return;
            case ENOTDIR:
                barf("401 ", "unauthorized"); break;
            default:
                barf("404 ", e_str(errno)); break;
        }
    }
    if (!filetype(fn.s, &contenttype)) die_nomem();

    rangefirst = 0;
    rangelast = filelength - 1;
    if (!httpdate(&mtimestr, mtime)) die_nomem();
    if (range.len > 0) {
        if (!rangeparser(&rangefirst, &rangelast, range.s, range.len, filelength)) {
            barf("416 ", "requested range not satisfiable");
        }

        header("206 ", "partial content");
        out_puts("Content-Range: bytes ");
        out_puts(numtostr(0, rangefirst));
        out_puts("-");
        out_puts(numtostr(0, rangelast));
        out_puts("/");
        out_puts(numtostr(0, filelength));
        out_puts("\r\n");
    }
    else {
        if ((ims.len < mtimestr.len) || byte_diff(mtimestr.s,mtimestr.len,ims.s)) {
            header("200 ", "OK");
        }
        else {
            header("304 ", "OK");
            flagbody = 0;
        }
    }

    if (seconds() > mtime + 60) {
        out_puts("Last-Modified:");
        out_put(mtimestr.s, mtimestr.len);
        out_puts("\r\n");
    }

    out_puts("Content-Type: ");
    out_put(contenttype.s, contenttype.len);
    out_puts("\r\nContent-Length: ");
    out_puts(numtostr(0, rangelast + 1 - rangefirst));
    out_puts("\r\n\r\n");
    out_body(filecontent + rangefirst, rangelast + 1 - rangefirst);
    close(fd);
    if (protocolnum < 2) _die(0);
}

static char inbuf[4096];
static long long inbuflen = 0;

static int getch(int fd, char *x) {

    long long r, i, j;
    char ch;

    if (inbuflen <= 0) {
        r = read(fd, inbuf, sizeof inbuf);
        if (r <= 0) return r;
        for (i = 0, j = r - 1; i < j; ++i, --j) {
            ch = inbuf[i];
            inbuf[i] = inbuf[j];
            inbuf[j] = ch;
        }
        inbuflen = r;
    }

    *x = inbuf[--inbuflen];
    return 1;
}

static void readline(void) {

    char ch;

    if (!stralloc_copys(&line, "")) die_nomem();

    for (;;) {
        if (getch(0, &ch) != 1) _die(0);
        if (!stralloc_append(&line, &ch)) die_nomem();
        if (ch == '\n') break;
    }
    if (!stralloc_0(&line)) die_nomem();
    --line.len;
    log_d2("request: ", line.s);

    if ((line.len > 0) && (line.s[line.len - 1] == '\n')) --line.len;
    if ((line.len > 0) && (line.s[line.len - 1] == '\r')) --line.len;
}

static int flagredir = 0;
static const char *user = 0;
static const char *root = 0;

static void usage(void) {

    log_u1("httpfile [options] [-r] [-u user] directory");
    _exit(100);
}

int main(int argc, char **argv) {

    (void) argc;

    void (*doit)(void) = get;
    char *x;
    long long i, spaces;

    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, drop);

    log_name("httpfile");


    if (!argv[0]) usage();
    for (;;) {
        if (!argv[1]) break;
        if (argv[1][0] != '-') break;
        x = *++argv;
        if (x[0] == '-' && x[1] == 0) break;
        if (x[0] == '-' && x[1] == '-' && x[2] == 0) break;
        while (*++x) {
            if (*x == 'q') { flagverbose = 0; log_level(flagverbose); continue; }
            if (*x == 'Q') { flagverbose = 1; log_level(flagverbose); continue; }
            if (*x == 'v') { log_level(++flagverbose); continue; }

            /* redirect */
            if (*x == 'r') { flagredir = 1; continue; }

            /* user */
            if (*x == 'u') {
                if (x[1]) { user = x + 1; break; }
                if (argv[1]) { user = *++argv; break; }
            }
            /* header */
            if (*x == 'h') {
                if (x[1]) { customheaders_add(x + 1); break; }
                if (argv[1]) { customheaders_add(*++argv); break; }
            }
            usage();
        }
    }
    root = *++argv;

    /* drop privileges */
    if (!droproot(user, root)) die_droproot();

    log_id(getenv("TCPREMOTEIP"));
    log_level(flagverbose);

    if (flagredir) doit = redirecthttps;

    if (!stralloc_readyplus(&response, 256)) die_nomem();
    if (!stralloc_readyplus(&field, 128)) die_nomem();

    for (;;) {
        alarm(REQUESTTIMEOUT);
        readline();

        if (line.len <= 0) continue;

        if (!stralloc_copys(&method, "")) die_nomem();
        if (!stralloc_copys(&url, "")) die_nomem();
        if (!stralloc_copys(&host, "")) die_nomem();
        if (!stralloc_copys(&path, "")) die_nomem();
        if (!stralloc_copys(&protocol, "")) die_nomem();
        if (!stralloc_copys(&ims, "")) die_nomem();
        if (!stralloc_copys(&auth, "")) die_nomem();
        if (!stralloc_copys(&range, "")) die_nomem();
        protocolnum = 2;

        spaces = 0;
        for (i = 0; i < line.len; ++i) {
            if (line.s[i] == ' ') {
                if (!i || (line.s[i - 1] != ' ')) {
                    ++spaces;
                    if (spaces >= 3) break;
                }
            }
            else {
                switch (spaces) {
                    case 0:
                        if (!stralloc_append(&method, line.s + i)) die_nomem();
                        break;
                    case 1:
                        if (!stralloc_append(&url, line.s + i)) die_nomem();
                        break;
                    case 2:
                        if (!stralloc_append(&protocol, line.s + i)) die_nomem();
                        break;
                }
            }
        }

        /* HTTP 0.9 not allowed */
        if (protocol.len <= 0) _die(111);

        if (!stralloc_0(&protocol)) die_nomem();
        if (case_equals(protocol.s, "http/1.0"))
            protocolnum = 1;

        if (!stralloc_0(&method)) die_nomem();
        flagbody = 1;
        if (str_equal(method.s, "HEAD"))
            flagbody = 0;
        else if (!str_equal(method.s, "GET"))
            barf("501 ","method not implemented");

        if (!stralloc_copy(&path, &url)) die_nomem();

        if (protocolnum > 0) {
            if (!stralloc_copys(&field, "")) die_nomem();
            for (;;) {
                readline();
                if (line.len <= 0 || ((line.s[0] != ' ') && (line.s[0] != '\t'))) {
                    if (case_startb(field.s,field.len,"content-length:")) {
                        barf("501 ","I do not accept messages");
                    }
                    if (case_startb(field.s,field.len,"transfer-encoding:")) {
                        barf("501 ","I do not accept messages");
                    }
                    if (case_startb(field.s,field.len,"expect:")) {
                        barf("417 ","I do not accept Expect");
                    }
                    if (case_startb(field.s,field.len,"host:")) {
                        if (host.len <= 0) {
                            for (i = 5; i < field.len; ++i) {
                                if (field.s[i] != ' ') {
                                    if (field.s[i] != '\t') {
                                        if (field.s[i] != '@') {
                                            if (!stralloc_append(&host, &field.s[i])) die_nomem();
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (case_startb(field.s, field.len, "if-modified-since:"))
                        if (!stralloc_copyb(&ims, field.s + 18, field.len - 18)) die_nomem();
                    if (case_startb(field.s, field.len, "authorization: basic ")) {
                        if (!stralloc_copyb(&auth, field.s + 21, field.len - 21)) die_nomem();
                    }
                    if (case_startb(field.s, field.len, "range: bytes=")) {
                        if (byte_chr(field.s + 13, field.len - 13, ',') == field.len - 13) {
                            if (!stralloc_copyb(&range, field.s + 13, field.len - 13)) die_nomem();
                        }
                    }

                    field.len = 0;
                }
                if (line.len <= 0) break;
                if (!stralloc_cat(&field, &line)) die_nomem();
            }
        }

        host.len = byte_chr(host.s, host.len, ':');
        if (!host.len) {
            if (protocolnum > 1) barf("400 ", "HTTP/1.1 requests must include a host name");
            if (!stralloc_copys(&host,"0")) die_nomem();
        }
        case_lowerb(host.s, host.len);

        alarm(0);
        doit();
    }
    return 111;
}
