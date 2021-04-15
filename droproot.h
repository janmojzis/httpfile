#ifndef _DROPROOT_H____
#define _DROPROOT_H____

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

extern int droproot(uid_t, gid_t);

#endif
