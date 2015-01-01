#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "strerr.h"
#include "fmt.h"
#include "buffer.h"

#define FATAL "httpfile-getmtime: fatal: "

void die_usage(void){strerr_die1x(100,"httpfile-getmtime: usage: httpfile-getmtime file");}
void die_stat(const char *str){strerr_die4sys(111,FATAL,"unable to stat file '",str,"': ");}
void die_write(void){strerr_die2sys(111,FATAL,"unable to write to output: ");}

char strnum[FMT_LONGLONG];

int main(int argc, char **argv) {

    struct stat st;

    if (!argv[0]) die_usage();
    if (!argv[1]) die_usage();

    if (stat(argv[1], &st) == -1) die_stat(argv[1]);

    if (buffer_put(buffer_1, strnum, fmt_longlong(strnum, st.st_mtime)) == -1) die_write();
    if (buffer_puts(buffer_1, "\n") == -1) die_write();
    if (buffer_flush(buffer_1) == -1) die_write();
    _exit(0);
}
