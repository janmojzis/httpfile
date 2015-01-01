/* Public domain. */

#include <poll.h>
#include <unistd.h>
#include "e.h"
#include "milliseconds.h"
#include "timeoutread.h"

long long timeoutread(long long t, int fd, char *buf, long long len) {

    long long tm;
    long long deadline;
    struct pollfd p;

    if (len < 0) { errno = EPROTO; return -1; }
    if (len > 1048576) len = 1048576;

    deadline = milliseconds() + t * 1000;

    p.fd = fd;
    p.events = POLLIN;
    for (;;) {
        tm = deadline - milliseconds();
        if (tm <= 0) {
            errno = ETIMEDOUT;
            return -1;
        }
        if (tm > 1000000) tm = 1000000;
        poll(&p, 1, tm);
        if (p.revents) break;
    }
    return read(fd, buf, len);
}
