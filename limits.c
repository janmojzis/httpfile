#include <sys/resource.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif
#include "log.h"
#include "limits.h"

int limits(void) {

#ifdef RLIM_INFINITY
    struct rlimit r;
    r.rlim_cur = 0;
    r.rlim_max = 0;
#endif
    int ret = 0;

    log_t1("jail()");

/* prohibit fork */
#ifdef RLIMIT_NPROC
    if (setrlimit(RLIMIT_NPROC, &r) == -1) {
        log_e1("unable to set RLIMIT_NPROC to 0");
        goto cleanup;
    }
#endif

/* prohibit core dumping */
#ifdef PR_SET_DUMPABLE
    if (prctl(PR_SET_DUMPABLE, 0) == -1) {
        log_e1("unable to set prctl(PR_SET_DUMPABLE, 0)");
        goto cleanup;
    }
#endif
#ifdef RLIMIT_CORE
    if (setrlimit(RLIMIT_CORE, &r) == -1) {
        log_e1("unable to set RLIMIT_CORE to 0");
        goto cleanup;
    }
#endif

    ret = 1;
cleanup:

    log_t2("jail() = ", lognum(ret));
    return ret;
}
