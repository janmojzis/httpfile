#ifndef _GETUIDGID_H____
#define _GETUIDGID_H____

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

extern int getuidgid(uid_t *, gid_t *, const char *);

#endif
