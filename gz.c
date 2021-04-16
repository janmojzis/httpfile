#include <zlib.h>
#include "alloc.h"
#include "gz.h"

char *gz(long long *outlen, char *in, long long inlen) {
    char *out = 0;
    z_stream strm = {0};
    long long alloclen = inlen + 20;
    int err;

    if (alloclen < 0) goto err;

    out = alloc(alloclen);
    if (!out) goto err;
    err = deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED,
              /* windows bits */ 15 | /* gzip */ 16,
              8,
              Z_DEFAULT_STRATEGY);
    if (err != Z_OK) goto err;

    strm.next_in = (unsigned char *)in;
    strm.avail_in = (uInt)inlen;

    strm.next_out = (unsigned char *)out;
    strm.avail_out = (uInt)alloclen;

    while (strm.avail_in > 0) {
        err = deflate(&strm, Z_NO_FLUSH);
        if (err != Z_OK) goto err;
    }

    err = deflate(&strm, Z_FINISH);
    if (err != Z_STREAM_END) goto err;

    err = deflateEnd(&strm);
    if (err != Z_OK) goto err;

    *outlen = strm.total_out;
    return out;

err:
    if (out) alloc_free(out);
    return 0;
}
