#include <sys/types.h>
#include <pwd.h>
#include "getuidgid.h"

int getuidgid(uid_t *uid, gid_t *gid, const char *account) {

    struct passwd *pw = 0;

	if (!account) return 0;

	pw = getpwnam(account);
	if (!pw) return 0;
	*uid = pw->pw_uid;
	*gid = pw->pw_gid;
    return 1;
}
