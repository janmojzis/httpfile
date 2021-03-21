#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

static char *fn;
static struct timeval t[2];

int main(int argc, char **argv) {

    (void) argc;

    if (!argv[0]) _exit(100);
    if (!argv[1]) _exit(100);
    if (!argv[2]) _exit(100);

    fn = argv[1];
    if (!fn) _exit(100);

    t[0].tv_sec = t[1].tv_sec = atol(argv[2]);

    if (utimes(fn, t) == -1) _exit(111);
    _exit(0);
}
