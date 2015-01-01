#include <unistd.h>
#include "open.h"
#include "strerr.h"

#define FATAL "httpfile-setlock: fatal: "

void die_usage(void){strerr_die1x(100,"httpfile-setlock: usage: httpfile-setlock lockfile child");}
void die_chdir(const char *str){strerr_die4sys(111,FATAL,"unable to change directory to '",str,"': ");}
void die_lock(const char *str){strerr_die4sys(111,FATAL,"unable to lock file '",str,"': ");}

int main(int argc,char **argv, char **env) {

    if (!argv[0]) die_usage();
    if (!argv[1]) die_usage();
    if (!argv[2]) die_usage();

    if (open_lock(argv[1]) == -1) die_lock(argv[1]);
    ++argv; ++argv;

    execvp(*argv, argv);
    strerr_die4sys(111,FATAL,"unable to run ",*argv,": ");
    return 111;
}
