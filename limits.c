#include <sys/resource.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif
#include "limits.h"

int limits(void) {

#ifdef PR_SET_DUMPABLE
    if (prctl(PR_SET_DUMPABLE, 0) == -1) return 0;
#endif

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
