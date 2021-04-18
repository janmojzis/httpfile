#define _LARGEFILE_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int file_open(char *fn, long long *mtime, long long *length) {

    struct stat st;
    int fd;

    fd = open(fn, O_RDONLY | O_NONBLOCK);
    if (fd == -1) return -1;

    if (fstat(fd, &st) == -1) {
        close(fd);
        return -1;
    }
    if ((st.st_mode & 0444) != 0444) {
        close(fd);
        errno = EACCES;
        return -1;
    }
    if ((st.st_mode & 0101) == 0001) {
        close(fd);
        errno = EACCES;
        return -1;
    }
    if ((st.st_mode & S_IFMT) != S_IFREG) {
        close(fd);
        errno = EISDIR;
        return -1;
    }

    *length = st.st_size;
    *mtime = st.st_mtime;

    /* probably useless overflow check */
    if (*length < 0 || *mtime < 0) {
        errno = EINVAL;
        return -1;
    }
    return fd;
}

int file_seek(int fd, long long offset) {
    if (offset < 0) { errno = EINVAL; return 0; }
    if (lseek(fd, offset, SEEK_SET) != offset) return 0;
    return 1;
}
