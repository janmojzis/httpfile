#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int file_open(char *fn, char **x, long long *xlen, long long *mtime) {

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

    *xlen = st.st_size;
    *mtime = st.st_mtime;

    if (st.st_size > 0) {
        *x = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (*x == MAP_FAILED) return -1;
    }

    return fd;
}

void file_close(int fd, char *x, long long xlen) {
    if (xlen > 0) {
        munmap(x, xlen);
    }
    close(fd);
}
