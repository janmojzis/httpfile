#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "buffer.h"
#include "e.h"
#include "file.h"
#include "byte.h"
#include "str.h"
#include "env.h"
#include "open.h"
#include "strerr.h"

#define FATAL "httpd: fatal: "
static void die_open(const char *str){strerr_die4sys(23,FATAL,"unable to open file '",str,"': ");}

static void log(char *fn,const char *result1,const char *result2)
{
  long long i;
  char ch;
  const char *x;

  x = env_get("TCPREMOTEIP");
  if (!x) x = "0";
  buffer_puts(buffer_2,x);
  buffer_puts(buffer_2," ");

  for (i = 0;i < 100;++i) {
    ch = fn[i];
    if (!ch) break;
    if (ch < 32) ch = '?';
    if (ch > 126) ch = '?';
    if (ch == ' ') ch = '_';
    buffer_put(buffer_2,&ch,1);
  }
  if (i == 100)
    buffer_puts(buffer_2,"...");

  buffer_puts(buffer_2,result1);
  buffer_puts(buffer_2,result2);
  buffer_puts(buffer_2,"\n");
  buffer_flush(buffer_2);
}

int file_open(char *fn, long long *mtime, long long *length) {

    struct stat st;
    int fd;

    fd = open_read(fn);
    if (fd == -1) {
        log(fn,": open failed: ",e_str(errno));
        if (e_temp(errno))  die_open(fn);
        return -1;
    }
    if (fstat(fd,&st) == -1) {
        log(fn,": fstat failed: ",e_str(errno));
        close(fd);
        if (e_temp(errno)) die_open(fn);
        return -1;
    }
    if ((st.st_mode & 0444) != 0444) {
        log(fn,": ","not ugo+r");
        close(fd);
        errno = EACCES;
        return -1;
    }
    if ((st.st_mode & 0101) == 0001) {
        log(fn,": ","o+x but u-x");
        close(fd);
        errno = EACCES;
        return -1;
    }
    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        /* log(fn,": ",e_str(EISDIR)); */
        close(fd);
        errno = EISDIR;
        return -1;
    }
    if ((st.st_mode & S_IFMT) != S_IFREG) {

        log(fn,": ","not a regular file");
        close(fd);
        errno = EACCES;
        return -1;
    }
    /* log(fn,": ","success"); */

    *length = st.st_size;
    *mtime  = st.st_mtime;

    return fd;
}

void file_log(char *fn) {
    log(fn,": ","success");
}

int file_seek(int fd, long long pos) {
    if ((long long)(off_t)pos != pos) return -1;
    return lseek(fd, pos, 0);
}

long long file_read(int fd, char *buf, long long len) {
    return read(fd,buf,len);
}
