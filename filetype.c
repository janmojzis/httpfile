#include "str.h"
#include "log.h"
#include "case.h"
#include "filetype.h"

int filetype(const char *fn, stralloc *contenttype) {
    const char *x;
    const char *result;
    long long i;
    char ch;
    int ret = 0;

    log_t3("filetype(fn = ", fn, ")");

    if (!stralloc_copys(contenttype, "")) goto cleanup;

    x = fn + str_rchr(fn, '.');
    if (x[str_chr(x, '=')]) {
        for (i = 0; (i < 100) && (ch = x[i]); ++i) {
            if ((ch != '=') && (ch != '-') && (ch != ':'))
                if ((ch < 'a') || (ch > 'z'))
                    if ((ch < '0') || (ch > '9')) continue;
            if (ch == '=') ch = '/';
            if (ch == ':') ch = '.';
            if (!stralloc_append(contenttype, &ch)) goto cleanup;
        }
    }
    else {
        result = "application/octet-stream";
        /* web content */
        if (case_equals(x, ".html")) result = "text/html";
        if (case_equals(x, ".css")) result = "text/css";
        if (case_equals(x, ".js")) result = "application/javascript";
        if (case_equals(x, ".xml")) result = "text/xml";
        /* archive + commpressed */
        if (case_equals(x, ".gz")) result = "application/gzip";
        if (case_equals(x, ".bz2")) result = "application/x-bzip2";
        if (case_equals(x, ".xz")) result = "application/x-xz";
        /* documents */
        if (case_equals(x, ".dvi")) result = "application/x-dvi";
        if (case_equals(x, ".ps")) result = "application/postscript";
        if (case_equals(x, ".pdf")) result = "application/pdf";
        /* images */
        if (case_equals(x, ".ico")) result = "image/x-icon";
        if (case_equals(x, ".gif")) result = "image/gif";
        if (case_equals(x, ".jpg")) result = "image/jpeg";
        if (case_equals(x, ".jpeg")) result = "image/jpeg";
        if (case_equals(x, ".png")) result = "image/png";
        /* video */
        if (case_equals(x, ".mpg")) result = "video/mpeg";
        if (case_equals(x, ".mpeg")) result = "video/mpeg";
        if (case_equals(x, ".flv")) result = "video/x-flv";
        if (case_equals(x, ".avi")) result = "video/x-msvideo";
        if (case_equals(x, ".mp4")) result = "video/mp4";
        if (case_equals(x, ".ogg")) result = "video/ogg";
        if (case_equals(x, ".mov")) result = "video/quicktime";
        if (case_equals(x, ".webm")) result = "video/webm";

        if (!stralloc_cats(contenttype, result)) goto cleanup;
    }
    ret = 1;
cleanup:
    if (!stralloc_0(contenttype)) {
        log_t1("filetype() = 0");
        return 0;
    }
    --contenttype->len;
    log_t6("filetype(fn = ", fn, ", contenttype = ", contenttype->s,
           ") = ", log_num(ret));
    return ret;
}
