#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif
#include "droproot.h"

int droproot(const char *account, const char *root) {

    struct passwd *pw;


    pw = getpwnam(account);
    if (!pw) { errno = ENOENT; return 0; }

#ifdef PR_SET_DUMPABLE
    if (prctl(PR_SET_DUMPABLE, 0) == -1) return 0;
#endif

    if (setgroups(1, &pw->pw_gid) == -1) return 0;
    if (setgid(pw->pw_gid) == -1) return 0;
    if (getgid() != pw->pw_gid) { errno = EPERM; return 0; }
    if (setuid(pw->pw_uid) == -1) return 0;
    if (getuid() != pw->pw_uid) { errno = EPERM; return 0; }

/* prohibit fork */
#ifdef RLIM_INFINITY
#ifdef RLIMIT_NPROC
    {
        struct rlimit r;
        r.rlim_cur = 0;
        r.rlim_max = 0;
        if (setrlimit(RLIMIT_NPROC, &r) == -1) return 0;
    }
#endif
#endif

    return 1;
}
