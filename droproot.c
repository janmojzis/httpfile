#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <errno.h>
#include "droproot.h"

int droproot(uid_t uid, gid_t gid) {

    if (setgroups(1, &gid) == -1) return 0;
    if (setgid(gid) == -1) return 0;
    if (getgid() != gid) {
        errno = EPERM;
        return 0;
    }
    if (setuid(uid) == -1) return 0;
    if (getuid() != uid) {
        errno = EPERM;
        return 0;
    }
    return 1;
}
