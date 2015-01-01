#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "open.h"

int open_lock(const char *fn) {

    int fd;

    fd = open(fn, O_RDWR | O_CREAT | O_NONBLOCK, 0600);
    if (fd == -1) return -1;
    if (lockf(fd, F_LOCK, 0) == -1) { close(fd); return -1; }
    return fd;
}
