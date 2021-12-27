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

    log_t1("limits()");

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

/* if memory limit is greater than 64MB */
/* set memory limit to 128MB            */
#define DATAMAX 134217728
#ifdef RLIMIT_DATA
    if (getrlimit(RLIMIT_DATA, &r) == -1) {
        log_e1("unable to get RLIMIT_DATA");
        goto cleanup;
    }
    if (r.rlim_cur > DATAMAX) {
        r.rlim_cur = r.rlim_max = DATAMAX;
        if (setrlimit(RLIMIT_DATA, &r) == -1) {
            log_e1("unable to set RLIMIT_DATA to 0");
            goto cleanup;
        }
        log_t2("setrlimit RLIMIT_DATA set to ", lognum(DATAMAX));
    }
#endif

    ret = 1;
cleanup:

    log_t2("limits() = ", lognum(ret));
    return ret;
}
